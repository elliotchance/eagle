#include <stdlib.h>
#include "EaglePlanJob.h"

EaglePlanJob* EaglePlanJob_New(EaglePlan *plan, int allocatedBuffers)
{
    EaglePlanJob *job = (EaglePlanJob*) malloc(sizeof(EaglePlanJob));
    
    job->plan = plan;
    
    // initialize all buffers now
    job->allocatedBuffers = allocatedBuffers;
    job->buffers = (EaglePage**) calloc(job->allocatedBuffers, sizeof(EaglePage));
    for(int i = 0; i < job->allocatedBuffers; ++i) {
        job->buffers[i] = EaglePage_Alloc(plan->pageSize);
    }
    
    return job;
}
