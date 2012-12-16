#include <stdlib.h>
#include <stdio.h>
#include "EagleWorker.h"
#include "EaglePlanJob.h"
#include "EagleInstance.h"

EagleWorker* EagleWorker_New(int workerId, struct EagleInstance_ *instance)
{
    EagleWorker *worker = (EagleWorker*) malloc(sizeof(EagleWorker));
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
            for(i = 0; i < job->plan->usedOperations; ++i) {
                EaglePlanOperation *epo = job->plan->operations[i];
                EaglePage *destination = NULL, *source1 = NULL, *source2 = NULL;
                
                /* prepare arguments */
                if(epo->destination >= 0) {
                    destination = job->buffers[epo->destination];
                }
                if(epo->source1 >= 0) {
                    source1 = job->buffers[epo->source1];
                }
                if(epo->source2 >= 0) {
                    source2 = job->buffers[epo->source2];
                }
                
                /* execute page operation */
                epo->function(destination, source1, source2, epo->obj);
            }
            
            /* extract records */
            for(i = 0; i < job->plan->pageSize; ++i) {
                if(job->buffers[0]->data[i]) {
                    EaglePageReceiver_pushRecordId(job->plan->receiver, job->buffers[0]->recordOffset + i);
                }
            }
            
            /* free */
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
    free(worker);
}
