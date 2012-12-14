#include <stdlib.h>
#include <stdio.h>
#include "EagleWorker.h"
#include "EaglePlanJob.h"
#include "EagleInstance.h"

EagleWorker* EagleWorker_New(int workerId, struct EagleInstance_ *instance)
{
    EagleWorker *worker = (EagleWorker*) malloc(sizeof(EagleWorker));
    worker->workerId = workerId;
    worker->thread = NULL;
    worker->instance = instance;
    return worker;
}

void* EagleWorker_begin(void *obj)
{
    EagleWorker *worker = (EagleWorker*) obj;
    
    while(1) {
        EaglePlanJob *job;
        
        /* ask the instance for the next job */
        job = EagleInstance_nextJob(worker->instance);
        
        /* run the job is one is returned */
        if(NULL != job) {
            int i;
            
            /* run operations */
            for(i = 0; i < job->plan->usedOperations; ++i) {
                EaglePlanOperation *epo = job->plan->operations[i];
                
                /* execute normal */
                if(epo->type == EaglePlanOperation_NORMAL) {
                    epo->function(job->buffers[epo->source], job->buffers[epo->destination], epo->obj);
                }
                
                /* execute page */
                else if(epo->type == EaglePlanOperation_PAGE) {
                    EaglePage *page = job->buffers[*((int*) epo->obj)];
                    epo->function(job->buffers[epo->source], job->buffers[epo->destination], (void*) page);
                }
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
    worker->thread = (pthread_t*) malloc(sizeof(pthread_t));
    pthread_create(worker->thread, NULL, EagleWorker_begin, worker);
}

void EagleWorker_join(EagleWorker *worker)
{
    pthread_join(*worker->thread, NULL);
}

/**
 This does not release the \c instance as that's only a reference to the parent object.
 */
void EagleWorker_Delete(EagleWorker *worker)
{
    free(worker->thread);
    free(worker);
}
