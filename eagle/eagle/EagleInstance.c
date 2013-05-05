#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "EagleInstance.h"
#include "EagleUtils.h"
#include "EagleMemory.h"
#include "EagleLogger.h"

EagleInstance* EagleInstance_New(int totalWorkers)
{
    EagleInstance *instance = (EagleInstance*) EagleMemory_Allocate("EagleInstance_New.1", sizeof(EagleInstance));
    if(NULL == instance) {
        return NULL;
    }
    
    instance->workers = EagleWorkers_New(totalWorkers, instance);
    instance->nextJobLock = EagleSynchronizer_CreateLock();
    instance->plan = NULL;
    
    return instance;
}

void EagleInstance_run(EagleInstance *eagle)
{
    /* start workers */
    EagleWorkers_start(eagle->workers);
    
    /* close workers */
    EagleWorkers_joinAll(eagle->workers);
}

void EagleInstance_addPlan(EagleInstance *eagle, EaglePlan *plan)
{
    eagle->plan = plan;
}

EaglePlanJob* EagleInstance_nextJob(EagleInstance *eagle)
{
    EaglePlan *plan = NULL;
    EaglePlanJob *job = NULL;
    uint64_t now, then;
    /*EagleBoolean sync = EagleTrue;*/
    
    plan = eagle->plan;
    /*EaglePlan_resumeTimer(plan);*/
    job = EaglePlanJob_New(plan);
    /*EaglePlan_stopTimer(plan);*/
    
    /* if all the providers can be accessed randomly we do not need to syncronise this function */
    /*EagleLinkedList_Foreach(eagle->plan->providers, EaglePlanBufferProvider*, provider)
    {
        switch(provider->type) {
                
            case EaglePlanBufferProviderTypeValue:
                * this case in find, we do not need sychronisation for this */
                /*break;
                
            case EaglePlanBufferProviderTypeProvider:
                if(EagleFalse == EaglePageProvider_isRandomAccess(provider->value.provider.provider)) {
                    sync = EagleFalse;
                }
                break;
                
        }
    }
    EagleLinkedList_ForeachEnd*/
    
    /*if(EagleTrue == sync) {*/
        /* synchronize this function */
        now = mach_absolute_time();
        EagleSynchronizer_Lock(eagle->nextJobLock);
        then = mach_absolute_time();
        plan->lockWaitTime += then - now;
    /*}*/
    
    /*EaglePlan_resumeTimer(plan);*/
    
    EagleLinkedList_Foreach(job->plan->providers, EaglePlanBufferProvider*, provider)
    {
        if(EaglePageProvider_pagesRemaining(provider->value.provider.provider) == 0) {
            EaglePlanJob_Delete(job);
            job = NULL;
            break;
        }
        
        if(provider->destinationBuffer >= plan->buffersNeeded) {
            char msg[1024];
            sprintf(msg, "destination %d is greater than allowed %d buffers!", provider->destinationBuffer, plan->buffersNeeded);
            EagleLogger_Log(EagleLoggerSeverityError, msg);
            
            EaglePlanJob_Delete(job);
            job = NULL;
            break;
        }
        EaglePage_Delete(job->buffers[provider->destinationBuffer]);
        job->buffers[provider->destinationBuffer] = EaglePageProvider_nextPage(provider->value.provider.provider);
    }
    EagleLinkedList_ForeachEnd
    
    EaglePlan_stopTimer(plan);
    
    /*if(EagleTrue == sync) {*/
        EagleSynchronizer_Unlock(eagle->nextJobLock);
    
    return job;
}

void EagleInstance_Delete(EagleInstance *eagle)
{
    if(NULL == eagle) {
        return;
    }
    
    EagleWorkers_Delete(eagle->workers);
    EagleLock_Delete(eagle->nextJobLock);
    EagleMemory_Free(eagle);
}
