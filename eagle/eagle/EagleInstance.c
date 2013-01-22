#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "EagleInstance.h"
#include "EagleUtils.h"
#include "EagleMemory.h"
#include "EagleLogger.h"

/**
 Create a new eagle instance. The instance is effectivly a database, with a certain amount of workers (threads) that
 will carry out the work. Do not create multiple instances for the same database as there is no way to syncronise them.
 @param [in] totalWorkers The number of workers to launch, this does not include the main thread that will handle the
 connections and look after the workers.
 */
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

/**
 Run the instance. This will start it.
 @param [in] eagle Instance from EagleInstance_New()
 */
void EagleInstance_run(EagleInstance *eagle)
{
    /* start workers */
    EagleWorkers_start(eagle->workers);
    
    /* close workers */
    EagleWorkers_joinAll(eagle->workers);
}

/**
 Add an EaglePlan to an EagleInstance.
 @param [in] eagle Instance.
 @param [in] plan The plan to execute.
 */
void EagleInstance_addPlan(EagleInstance *eagle, EaglePlan *plan)
{
    eagle->plan = plan;
}

/**
 Get the next finite job from the stack. This is a syncronised method that all the workers will call independantly when
 they have nothing to do. If this method returns NULL it is upto the workers to know that there is no more work, they
 will check periodically after that.
 @param [in] eagle Instance.
 @return Initialised EaglePlanJob, or NULL if there are no available jobs.
 */
EaglePlanJob* EagleInstance_nextJob(EagleInstance *eagle)
{
    EaglePlan *plan = NULL;
    EaglePlanJob *job = NULL;
    int i;
    uint64_t now, then;
    
    /* synchronize this function */
    now = mach_absolute_time();
    EagleSynchronizer_Lock(eagle->nextJobLock);
    then = mach_absolute_time();
    
    plan = eagle->plan;
    plan->lockWaitTime += then - now;
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
            EagleLogger_Log(EagleLoggerSeverityError, msg);
            return NULL;
        }
        EaglePage_Delete(job->buffers[provider->destinationBuffer]);
        job->buffers[provider->destinationBuffer] = EaglePageProvider_nextPage(provider->provider);
    }
    
    EagleSynchronizer_Unlock(eagle->nextJobLock);
    EaglePlan_stopTimer(plan);
    return job;
}

/**
 Free EagleInstance
 @param [in] eagle Instance.
 */
void EagleInstance_Delete(EagleInstance *eagle)
{
    if(NULL == eagle) {
        return;
    }
    
    EagleWorkers_Delete(eagle->workers);
    EagleLock_Delete(eagle->nextJobLock);
    EaglePlan_Delete(eagle->plan);
    EagleMemory_Free(eagle);
}
