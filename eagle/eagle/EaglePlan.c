#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "EaglePlan.h"
#include "EagleUtils.h"

EaglePlan* EaglePlan_New(int pageSize)
{
    EaglePlan *plan = (EaglePlan*) malloc(sizeof(EaglePlan));
    
    plan->pageSize = pageSize;
    
    plan->allocatedOperations = 10;
    plan->usedOperations = 0;
    plan->operations = (EaglePlanOperation**) calloc((size_t) plan->allocatedOperations, sizeof(EaglePlanOperation*));
    
    plan->allocatedProviders = 10;
    plan->usedProviders = 0;
    plan->providers = (EaglePlanBufferProvider**) calloc((size_t) plan->allocatedProviders, sizeof(EaglePlanBufferProvider*));
    
    plan->errorCode = EaglePlanErrorNone;
    plan->errorMessage = NULL;
    plan->resultFields = 0;
    
    return plan;
}

void EaglePlan_addOperation(EaglePlan *plan, EaglePlanOperation *epo)
{
    plan->operations[plan->usedOperations++] = epo;
}

void EaglePlan_addBufferProvider(EaglePlan *plan, EaglePlanBufferProvider *bp)
{
    plan->providers[plan->usedProviders++] = bp;
}

const char* EaglePlan_toString(EaglePlan *plan)
{
    char *str = (char*) malloc(1024), *temp;
    int i;
    
    str[0] = 0;
    strcat_safe(str, "EaglePlan:\n");
    
    if(plan->usedProviders > 0) {
        strcat_safe(str, "  Providers:\n");
    }
    for(i = 0; i < plan->usedProviders; ++i) {
        strcat_safe(str, "    ");
        temp = EaglePlanBufferProvider_toString(plan->providers[i]);
        strcat_safe(str, temp);
        free(temp);
        strcat_safe(str, "\n");
    }
    
    if(plan->usedOperations > 0) {
        strcat_safe(str, "  Operations:\n");
    }
    for(i = 0; i < plan->usedOperations; ++i) {
        strcat_safe(str, "    ");
        strcat_safe(str, EaglePlanOperation_toString(plan->operations[i]));
        strcat_safe(str, "\n");
    }
    
    return str;
}

void EaglePlan_Delete(EaglePlan *plan)
{
    int i;
    
    if(NULL == plan) {
        return;
    }
    
    for(i = 0; i < plan->usedOperations; ++i) {
        EaglePlanOperation_Delete(plan->operations[i]);
    }
    free(plan->operations);
    
    for(i = 0; i < plan->usedProviders; ++i) {
        EaglePlanBufferProvider_Delete(plan->providers[i]);
    }
    free(plan->providers);
    
    free(plan->errorMessage);
    free(plan);
    plan = NULL;
}

EaglePlanBufferProvider* EaglePlan_getBufferProviderByName(EaglePlan *plan, char *name)
{
    int i;
    
    for(i = 0; i < plan->usedProviders; ++i) {
        EaglePageProvider *provider = plan->providers[i]->provider;
        if(NULL != provider->name && strcmp(name, provider->name) == 0) {
            return plan->providers[i];
        }
    }
    
    return NULL;
}

void EaglePlan_setError(EaglePlan *plan, EaglePlanError errorCode, char *errorMessage)
{
    plan->errorCode = errorCode;
    plan->errorMessage = strdup(errorMessage);
}

EagleBoolean EaglePlan_isError(EaglePlan *plan)
{
    if(plan->errorCode == EaglePlanErrorNone) {
        return EagleFalse;
    }
    return EagleTrue;
}
