#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "EagleInstance.h"
#include "EagleUtils.h"

EagleInstance* EagleInstance_New(int totalWorkers)
{
    EagleInstance *instance = (EagleInstance*) malloc(sizeof(EagleInstance));
    
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

/**
 Add an EaglePlan to an EagleInstance.
 */
void EagleInstance_addPlan(EagleInstance *eagle, EaglePlan *plan)
{
    eagle->plan = plan;
}

EaglePlanJob* EagleInstance_nextJob(EagleInstance *eagle)
{
    EaglePlan *plan = NULL;
    EaglePlanJob *job = NULL;
    int i;
    
    /* synchronize this function */
    EagleSynchronizer_Lock(eagle->nextJobLock);
    
    plan = eagle->plan;
    EaglePlan_resumeTimer(plan);
    job = EaglePlanJob_New(plan);
    
    for(i = 0; i < plan->usedProviders; ++i) {
        EaglePlanBufferProvider *provider = plan->providers[i];
        if(EaglePageProvider_pagesRemaining(provider->provider) == 0) {
            EaglePlanJob_Delete(job);
            job = NULL;
            break;
        }
        
        if(provider->destinationBuffer >= plan->buffersNeeded) {
            char msg[1024];
            sprintf(msg, "destination %d is greater than allowed %d buffers!\n", provider->destinationBuffer, plan->buffersNeeded);
            EagleUtils_Fatal(msg);
        }
        EaglePage_Delete(job->buffers[provider->destinationBuffer]);
        job->buffers[provider->destinationBuffer] = EaglePageProvider_nextPage(provider->provider);
    }
    
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
    EaglePlan_Delete(eagle->plan);
    free(eagle);
}
