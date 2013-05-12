#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "EaglePlan.h"
#include "EagleUtils.h"
#include "EagleMemory.h"
#include "EaglePageProvider.h"

EaglePlan* EaglePlan_New(int pageSize, int cores)
{
    int i;
    EaglePlan *plan = (EaglePlan*) EagleMemory_Allocate("EaglePlan_New.1", sizeof(EaglePlan));
    if(NULL == plan) {
        return NULL;
    }
    
    plan->pageSize = pageSize;
    plan->operations = EagleLinkedList_New();
    plan->providers = EagleLinkedList_New();
    plan->errorCode = EaglePlanErrorNone;
    plan->errorMessage = NULL;
    
    /* timers */
    plan->cores = cores;
    plan->executionTime = (uint64_t*) EagleMemory_MultiAllocate("EaglePlan_New.2", sizeof(uint64_t), cores);
    plan->lockTime = (uint64_t*) EagleMemory_MultiAllocate("EaglePlan_New.3", sizeof(uint64_t), cores);
    plan->startTime = 0;
    
    for(i = 0; i < cores; ++i) {
        plan->executionTime[i] = 0;
        plan->lockTime[i] = 0;
    }
    
    plan->buffersNeeded = 0;
    plan->bufferTypes = NULL;
    
    plan->resultFields = 0;
    plan->result = NULL;
    
    plan->freeObjects = EagleLinkedList_New();
    
    return plan;
}

void EaglePlan_addOperation(EaglePlan *plan, EaglePlanOperation *epo)
{
    EagleLinkedList_addObject(plan->operations, epo, EagleFalse, NULL);
}

void EaglePlan_addBufferProvider(EaglePlan *plan, EaglePlanBufferProvider *bp, EagleBoolean free)
{
    EagleLinkedList_addObject(plan->providers, bp, free, (void(*)(void*)) EaglePlanBufferProvider_Delete);
}

void EaglePlan_addFreeObject(EaglePlan *plan, void *obj, void (*free)(void*))
{
    EagleLinkedListItem *item = EagleLinkedListItem_New(obj, EagleTrue, free);
    EagleLinkedList_add(plan->freeObjects, item);
}

const char* EaglePlan_toString(EaglePlan *plan)
{
    char *str, *temp = NULL;
    int i;
    
    if(NULL == plan) {
        return NULL;
    }
    
    str = (char*) EagleMemory_Allocate("EaglePlan_toString.1", 1024);
    if(NULL == str) {
        return NULL;
    }
    
    str[0] = 0;
    strcat_safe(str, "EaglePlan:\n");
    
    if(!EagleLinkedList_isEmpty(plan->providers)) {
        strcat_safe(str, "  Input Providers:\n");
        
        EagleLinkedList_Foreach(plan->providers, EaglePlanBufferProvider*, provider)
        {
            strcat_safe(str, "    ");
            temp = EaglePlanBufferProvider_toString(provider);
            strcat_safe(str, temp);
            EagleMemory_Free(temp);
            strcat_safe(str, "\n");
        }
        EagleLinkedList_ForeachEnd
    }
    
    if(plan->resultFields > 0) {
        strcat_safe(str, "  Output Providers:\n");
        
        for(i = 0; i < plan->resultFields; ++i) {
            EaglePageProvider *provider = plan->result[i];
            
            strcat_safe(str, "    destination = ");
            sprintf(str, "%s%d", str, i);
            
            strcat_safe(str, ", name = ");
            strcat_safe(str, provider->name);
            
            strcat_safe(str, ", type = ");
            temp = EagleDataType_typeToName(provider->type);
            strcat_safe(str, temp);
            EagleMemory_Free(temp);
            
            strcat_safe(str, "\n");
        }
    }
    
    if(!EagleLinkedList_isEmpty(plan->operations)) {
        strcat_safe(str, "  Operations:\n");
        
        EagleLinkedList_Foreach(plan->operations, EaglePlanOperation*, op)
        {
            char *s = EaglePlanOperation_toString(op);
            strcat_safe(str, "    ");
            strcat_safe(str, s);
            strcat_safe(str, "\n");
            EagleMemory_Free(s);
        }
        EagleLinkedList_ForeachEnd
    }
    
    if(plan->buffersNeeded > 0) {
        strcat_safe(str, "  Buffers:\n");
        
        for(i = 0; i < plan->buffersNeeded; ++i) {
            char *msg = (char*) EagleMemory_Allocate("EaglePlan_toString.2", 128), *type;
            if(NULL == msg) {
                EagleMemory_Free(str);
                return NULL;
            }
            
            type = EagleDataType_typeToName(plan->bufferTypes[i]);
            sprintf(msg, "    %d type=%s\n", i, type);
            strcat_safe(str, msg);
            EagleMemory_Free(type);
            EagleMemory_Free(msg);
        }
    }
    
    return str;
}

void EaglePlan_Delete(EaglePlan *plan)
{
    if(NULL == plan) {
        return;
    }
    
    EagleMemory_Free(plan->executionTime);
    
    EagleLinkedList_DeleteWithItems(plan->freeObjects);
    EagleMemory_Free(plan->result);
    EagleLinkedList_DeleteWithItems(plan->providers);
    EagleLinkedList_DeleteWithItems(plan->operations);
    EagleMemory_Free(plan->errorMessage);
    EagleMemory_Free(plan->bufferTypes);
    EagleMemory_Free(plan);
}

EaglePlanBufferProvider* EaglePlan_getBufferProviderByName(EaglePlan *plan, char *name)
{
    EagleLinkedList_Foreach(plan->providers, EaglePlanBufferProvider*, provider)
    {
        if(NULL != provider->value.provider.provider->name && strcmp(name, provider->value.provider.provider->name) == 0) {
            return provider;
        }
    }
    EagleLinkedList_ForeachEnd
    
    return NULL;
}

void EaglePlan_setError(EaglePlan *plan, EaglePlanError errorCode, char *errorMessage)
{
    plan->errorCode = errorCode;
    plan->errorMessage = (NULL == errorMessage ? NULL : strdup(errorMessage));
}

EagleBoolean EaglePlan_isError(EaglePlan *plan)
{
    if(plan->errorCode == EaglePlanErrorNone) {
        return EagleFalse;
    }
    return EagleTrue;
}

double EaglePlan_getRealExecutionSeconds(EaglePlan *plan)
{
    uint64_t elapsed = mach_absolute_time() - plan->startTime;
    
    /* the timer was not started - bad */
    if(0 == plan->startTime) {
        return 0.0;
    }
    
    return ((double) elapsed * 1.0e-9);
}

double EaglePlan_getLockSeconds(EaglePlan *plan)
{
    uint64_t total = 0.0;
    int i;
    
    for(i = 0; i < plan->cores; ++i) {
        total += plan->lockTime[i];
    }
    
    return ((double) total * 1.0e-9);
}

double EaglePlan_getExecutionSeconds(EaglePlan *plan)
{
    uint64_t total = 0.0;
    int i;
    
    for(i = 0; i < plan->cores; ++i) {
        total += plan->executionTime[i];
    }
    
    return ((double) total * 1.0e-9) - EaglePlan_getLockSeconds(plan);
}

void EaglePlan_prepareBuffers(EaglePlan *plan, int buffers)
{
    int i;
    
    if(NULL == plan) {
        return;
    }
    
    plan->buffersNeeded = buffers;
    plan->bufferTypes = (EagleDataType*) EagleMemory_MultiAllocate("EaglePlan_prepareBuffers.1", sizeof(EagleDataType), plan->buffersNeeded);
    if(NULL == plan->bufferTypes) {
        return;
    }
    for(i = 0; i < plan->buffersNeeded; ++i) {
        plan->bufferTypes[i] = EagleDataTypeUnknown;
    }
}
