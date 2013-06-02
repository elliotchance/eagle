#include <stdlib.h>
#include <stdio.h>
#include "EagleWorker.h"
#include "EaglePlanJob.h"
#include "EagleInstance.h"
#include "EagleUtils.h"
#include "EagleMemory.h"
#include "EagleLogger.h"

/**
 Used by EagleWorker_GetForCurrentThread() and EagleWorker_SetForCurrentThread()
 */
__thread EagleWorker *EagleWorker_ThisWorker = NULL;

EagleWorker* EagleWorker_New(int workerId, struct EagleInstance *instance)
{
    EagleWorker *worker = (EagleWorker*) EagleMemory_Allocate("EagleWorker_New.1", sizeof(EagleWorker));
    if(NULL == worker) {
        return NULL;
    }
    
    worker->workerId = workerId;
    worker->instance = instance;
    worker->lockTime = 0;
    
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
    EagleWorker_SetForCurrentThread(worker);
    
    while(1) {
        EaglePlanJob *job = NULL;
        uint64_t start;
        
        /* start the timers at zero */
        start = EagleUtils_GetAbsoluteTime();
        worker->lockTime = 0;
        
        /* ask the instance for the next job */
        job = EagleInstance_nextJob(worker->instance, worker->workerId);
        
        /* run the job is one is returned */
        if(NULL != job) {
            /* is this the start of the plan itself? */
            if(0 == job->plan->startTime) {
                job->plan->startTime = start;
            }
                
            /* run operations */
            EagleWorker_runJob(job);
            
            /* add time */
            job->plan->executionTime[worker->workerId] += EagleUtils_GetAbsoluteTime() - start;
            job->plan->lockTime[worker->workerId] += worker->lockTime;
            
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

void EagleWorker_Delete(EagleWorker *worker)
{
    if(NULL == worker) {
        return;
    }
    
    EagleMemory_Free(worker);
}

EagleWorker* EagleWorker_GetForCurrentThread(void)
{
    return EagleWorker_ThisWorker;
}

void EagleWorker_SetForCurrentThread(EagleWorker *worker)
{
    EagleWorker_ThisWorker = worker;
}
