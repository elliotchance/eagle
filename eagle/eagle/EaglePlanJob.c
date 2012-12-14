#include <stdlib.h>
#include "EaglePlanJob.h"

EaglePlanJob* EaglePlanJob_New(EaglePlan *plan, int allocatedBuffers)
{
    EaglePlanJob *job = (EaglePlanJob*) malloc(sizeof(EaglePlanJob));
    int i;
    
    job->plan = plan;
    
    /* initialize all buffers now */
    job->allocatedBuffers = allocatedBuffers;
    job->buffers = (EaglePage**) calloc((size_t) job->allocatedBuffers, sizeof(EaglePage));
    for(i = 0; i < job->allocatedBuffers; ++i) {
        job->buffers[i] = EaglePage_Alloc(plan->pageSize);
    }
    
    return job;
}

void EaglePlanJob_Delete(EaglePlanJob *job)
{
    int i;
    
    for(i = 0; i < job->allocatedBuffers; ++i) {
        EaglePage_Delete(job->buffers[i]); 
    }
    free((void*) job->buffers);
    
    free((void*) job);
    job = NULL;
}
