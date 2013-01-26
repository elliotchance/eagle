#include <stdlib.h>
#include "EaglePlanJob.h"
#include "EagleMemory.h"

EaglePlanJob* EaglePlanJob_New(EaglePlan *plan)
{
    int i;
    EaglePlanJob *job;
    
    if(NULL == plan) {
        return NULL;
    }
    
    job = (EaglePlanJob*) EagleMemory_Allocate("EaglePlanJob_New.1", sizeof(EaglePlanJob));
    if(NULL == job) {
        return NULL;
    }
    
    job->plan = plan;
    
    /* initialize all buffers now */
    job->buffers = (EaglePage**) EagleMemory_MultiAllocate("EaglePlanJob_New.2", sizeof(EaglePage*), plan->buffersNeeded);
    if(NULL == job->buffers){
        EaglePlanJob_Delete(job);
        return NULL;
    }
    for(i = 0; i < plan->buffersNeeded; ++i) {
        job->buffers[i] = EaglePage_Alloc(plan->bufferTypes[i], plan->pageSize);
    }
    
    return job;
}

void EaglePlanJob_Delete(EaglePlanJob *job)
{
    int i;
    
    if(NULL == job) {
        return;
    }
    
    if(NULL != job->buffers) {
        for(i = 0; i < job->plan->buffersNeeded; ++i) {
            EaglePage_Delete(job->buffers[i]);
        }
        EagleMemory_Free(job->buffers);
    }
    
    EagleMemory_Free(job);
}
