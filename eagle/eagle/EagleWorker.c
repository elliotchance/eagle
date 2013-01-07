#include <stdlib.h>
#include <stdio.h>
#include "EagleWorker.h"
#include "EaglePlanJob.h"
#include "EagleInstance.h"
#include "EagleUtils.h"
#include "EagleMemory.h"
#include "EagleLogger.h"

EagleWorker* EagleWorker_New(int workerId, struct EagleInstance_ *instance)
{
    EagleWorker *worker = (EagleWorker*) EagleMemory_Allocate("EagleWorker_New.1", sizeof(EagleWorker));
    if(NULL == worker) {
        return NULL;
    }
    
    worker->workerId = workerId;
    worker->instance = instance;
    
    return worker;
}

void* EagleWorker_begin(void *obj)
{
    EagleWorker *worker = (EagleWorker*) obj;
    
    while(1) {
        EaglePlanJob *job = NULL;
        
        /* ask the instance for the next job */
        job = EagleInstance_nextJob(worker->instance);
        
        /* run the job is one is returned */
        if(NULL != job) {
            int i;
            
            /* run operations */
            EaglePlan_resumeTimer(job->plan);
            for(i = 0; i < job->plan->usedOperations; ++i) {
                EaglePlanOperation *epo = job->plan->operations[i];
                EaglePage *destination = NULL, *source1 = NULL, *source2 = NULL;
                
                /* prepare arguments */
                if(epo->destination >= 0) {
                    if(epo->destination >= job->plan->buffersNeeded) {
                        char msg[1024];
                        sprintf(msg, "destination %d is greater than allowed %d buffers!\n", epo->destination, job->plan->buffersNeeded);
                        EagleLogger_Log(EagleLoggerSeverityError, msg);
                        return NULL;
                    }
                    destination = job->buffers[epo->destination];
                }
                if(epo->source1 >= 0) {
                    if(epo->source1 >= job->plan->buffersNeeded) {
                        char msg[1024];
                        sprintf(msg, "destination %d is greater than allowed %d buffers!\n", epo->source1, job->plan->buffersNeeded);
                        EagleLogger_Log(EagleLoggerSeverityError, msg);
                        return NULL;
                    }
                    source1 = job->buffers[epo->source1];
                }
                if(epo->source2 >= 0) {
                    if(epo->source2 >= job->plan->buffersNeeded) {
                        char msg[1024];
                        sprintf(msg, "destination %d is greater than allowed %d buffers!\n", epo->source2, job->plan->buffersNeeded);
                        EagleLogger_Log(EagleLoggerSeverityError, msg);
                        return NULL;
                    }
                    source2 = job->buffers[epo->source2];
                }
                
                /* execute page operation */
                epo->function(destination, source1, source2, epo->obj);
            }
            
            /* free */
            EaglePlan_stopTimer(job->plan);
            EaglePlanJob_Delete(job);
        }
        else {
            return NULL;
        }
    }
    
    return NULL;
}

void EagleWorker_start(EagleWorker *worker)
{
    pthread_create(&worker->thread, NULL, EagleWorker_begin, worker);
}

void EagleWorker_join(EagleWorker *worker)
{
    pthread_join(worker->thread, NULL);
}

/**
 This does not release the \c instance as that's only a reference to the parent object.
 */
void EagleWorker_Delete(EagleWorker *worker)
{
    EagleMemory_Free(worker);
}
