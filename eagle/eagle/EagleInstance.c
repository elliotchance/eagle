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
    
    /* synchronize this function */
    now = mach_absolute_time();
    EagleSynchronizer_Lock(eagle->nextJobLock);
    then = mach_absolute_time();
    
    plan = eagle->plan;
    plan->lockWaitTime += then - now;
    EaglePlan_resumeTimer(plan);
    job = EaglePlanJob_New(plan);
    
    EagleLinkedList_Foreach(job->plan->providers, EaglePlanBufferProvider*, provider)
    {
        if(EaglePageProvider_pagesRemaining(provider->provider) == 0) {
            EaglePlanJob_Delete(job);
            job = NULL;
            break;
        }
        
        if(provider->destinationBuffer >= plan->buffersNeeded) {
            char msg[1024];
            sprintf(msg, "destination %d is greater than allowed %d buffers!\n", provider->destinationBuffer, plan->buffersNeeded);
            EagleLogger_Log(EagleLoggerSeverityError, msg);
            
            EaglePlanJob_Delete(job);
            job = NULL;
            break;
        }
        EaglePage_Delete(job->buffers[provider->destinationBuffer]);
        job->buffers[provider->destinationBuffer] = EaglePageProvider_nextPage(provider->provider);
    }
    EagleLinkedList_ForeachEnd
    
    EagleSynchronizer_Unlock(eagle->nextJobLock);
    EaglePlan_stopTimer(plan);
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
