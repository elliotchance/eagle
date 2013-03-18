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

void EagleWorker_runJobLiteral_(EaglePlanJob *job, EaglePlanOperation *epo)
{
    EaglePage *destination = NULL, *source1 = NULL;
    EagleDbSqlValue *value;
    
    /* prepare arguments */
    if(epo->destination >= 0) {
        if(epo->destination >= job->plan->buffersNeeded) {
            char msg[1024];
            sprintf(msg, "destination %d is greater than allowed %d buffers!", epo->destination, job->plan->buffersNeeded);
            EagleLogger_Log(EagleLoggerSeverityError, msg);
            return;
        }
        destination = job->buffers[epo->destination];
    }
    if(epo->source1 >= 0) {
        if(epo->source1 >= job->plan->buffersNeeded) {
            char msg[1024];
            sprintf(msg, "source1 %d is greater than allowed %d buffers!", epo->source1, job->plan->buffersNeeded);
            EagleLogger_Log(EagleLoggerSeverityError, msg);
            return;
        }
        source1 = job->buffers[epo->source1];
    }
    
    /* execute page operation */
    value = (EagleDbSqlValue*) epo->obj;
    switch(value->type) {
            
        case EagleDbSqlValueTypeAsterisk:
        case EagleDbSqlValueTypeIdentifier:
            /* these are bogus and should never occur */
            break;
            
        case EagleDbSqlValueTypeFloat:
            epo->function(destination, source1, NULL, &value->value.floatValue);
            break;
            
        case EagleDbSqlValueTypeInteger:
            epo->function(destination, source1, NULL, &value->value.intValue);
            break;
            
        case EagleDbSqlValueTypeString:
            epo->function(destination, source1, NULL, &value->value.identifier);
            break;
            
    }
}

void EagleWorker_runJobPage_(EaglePlanJob *job, EaglePlanOperation *epo)
{
    EaglePage *destination = NULL, *source1 = NULL, *source2 = NULL;
    
    /* prepare arguments */
    if(epo->destination >= 0) {
        if(epo->destination >= job->plan->buffersNeeded) {
            char msg[1024];
            sprintf(msg, "destination %d is greater than allowed %d buffers!", epo->destination, job->plan->buffersNeeded);
            EagleLogger_Log(EagleLoggerSeverityError, msg);
            return;
        }
        destination = job->buffers[epo->destination];
    }
    if(epo->source1 >= 0) {
        if(epo->source1 >= job->plan->buffersNeeded) {
            char msg[1024];
            sprintf(msg, "source1 %d is greater than allowed %d buffers!", epo->source1, job->plan->buffersNeeded);
            EagleLogger_Log(EagleLoggerSeverityError, msg);
            return;
        }
        source1 = job->buffers[epo->source1];
    }
    if(epo->source2 >= 0) {
        if(epo->source2 >= job->plan->buffersNeeded) {
            char msg[1024];
            sprintf(msg, "source2 %d is greater than allowed %d buffers!", epo->source2, job->plan->buffersNeeded);
            EagleLogger_Log(EagleLoggerSeverityError, msg);
            return;
        }
        source2 = job->buffers[epo->source2];
    }
    
    /* execute page operation */
    epo->function(destination, source1, source2, epo->obj);
}

void EagleWorker_runJob(EaglePlanJob *job)
{
    EaglePlan_resumeTimer(job->plan);
    
    EagleLinkedList_Foreach(job->plan->operations, EaglePlanOperation*, epo)
    {
        switch(epo->type) {
                
            case EaglePlanOperationTypePage:
                EagleWorker_runJobPage_(job, epo);
                break;
                
            case EaglePlanOperationTypeLiteral:
                EagleWorker_runJobLiteral_(job, epo);
                break;
                
        }
        
    }
    EagleLinkedList_ForeachEnd
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
            /* run operations */
            EagleWorker_runJob(job);
            
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

void EagleWorker_Delete(EagleWorker *worker)
{
    if(NULL == worker) {
        return;
    }
    
    EagleMemory_Free(worker);
}
