#include <stdlib.h>
#include <stdio.h>
#include "EagleInstance.h"

EagleInstance* EagleInstance_New(int totalWorkers)
{
    EagleInstance *instance = (EagleInstance*) malloc(sizeof(EagleInstance));
    instance->workers = EagleWorkers_New(totalWorkers, instance);
    instance->nextJobLock = EagleSynchronizer_CreateLock();
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
    job = EaglePlanJob_New(plan, 4);
    
    for(i = 0; i < plan->usedProviders; ++i) {
        EaglePlanBufferProvider *provider = plan->providers[i];
        if(EaglePageProvider_pagesRemaining(provider->provider) == 0) {
            EaglePlanJob_Delete(job);
            job = NULL;
            break;
        }
        job->buffers[provider->destinationBuffer] = EaglePageProvider_nextPage(provider->provider);
    }
    
    EagleSynchronizer_Unlock(eagle->nextJobLock);
    return job;
}

void EagleInstance_Delete(EagleInstance *eagle)
{
    EagleWorkers_Delete(eagle->workers);
    EagleLock_Delete(eagle->nextJobLock);
    EaglePlan_Delete(eagle->plan);
}
