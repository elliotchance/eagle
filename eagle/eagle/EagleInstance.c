#include <stdlib.h>
#include <stdio.h>
#include "EagleInstance.h"

EagleInstance* EagleInstance_New(int totalWorkers)
{
    EagleInstance *instance = (EagleInstance*) malloc(sizeof(EagleInstance));
    instance->workers = EagleWorkers_New(totalWorkers, instance);
    instance->nextJobLock = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
    return instance;
}

void EagleInstance_run(EagleInstance *eagle)
{
    /* start workers */
    pthread_mutex_init(eagle->nextJobLock, NULL);
    EagleWorkers_start(eagle->workers);
    
    /* close workers */
    EagleWorkers_joinAll(eagle->workers);
    pthread_mutex_destroy(eagle->nextJobLock);
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
    EaglePlan *plan;
    EaglePlanJob *job;
    int i;
    
    /* synchronize this function */
    pthread_mutex_lock(eagle->nextJobLock);
    
    plan = eagle->plan;
    job = EaglePlanJob_New(plan, 4);
    
    for(i = 0; i < plan->usedProviders; ++i) {
        EaglePlanBufferProvider *provider = plan->providers[i];
        if(EaglePageProvider_pagesRemaining(provider->provider) == 0) {
            pthread_mutex_unlock(eagle->nextJobLock);
            return NULL;
        }
        job->buffers[provider->destinationBuffer] = EaglePageProvider_nextPage(provider->provider);
    }
    
    pthread_mutex_unlock(eagle->nextJobLock);
    return job;
}