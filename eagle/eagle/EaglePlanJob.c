#include <stdlib.h>
#include "EaglePlanJob.h"

EaglePlanJob* EaglePlanJob_New(EaglePlan *plan)
{
    int i;
    EaglePlanJob *job = (EaglePlanJob*) malloc(sizeof(EaglePlanJob));
    
    job->plan = plan;
    
    /* initialize all buffers now */
    job->buffers = (EaglePage**) calloc((size_t) plan->buffersNeeded, sizeof(EaglePage*));
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
    
    for(i = 0; i < job->plan->buffersNeeded; ++i) {
        EaglePage_Delete(job->buffers[i]);
    }
    free((void*) job->buffers);
    
    free((void*) job);
    job = NULL;
}
