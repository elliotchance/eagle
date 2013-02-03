#ifndef eagle_EaglePlanJob_h
#define eagle_EaglePlanJob_h

#include "EaglePlan.h"
#include "Eagle.h"

/**
 A plan is divided up into single jobs that are run simultaneously.
 */
typedef struct {
    
    /**
     A reference to the plan.
     */
    EAGLE_ATTR_PROVIDED EaglePlan *plan;
    
    /**
     The buffers.
     */
    EAGLE_ATTR_MANAGED EaglePage **buffers;
    
} EaglePlanJob;

/**
 * Create a new job from a plan.
 * @param [in] plan The plan template.
 * @return A new job.
 */
EaglePlanJob* EaglePlanJob_New(EaglePlan *plan);

/**
 * Delete a job.
 * @param [in] job The job.
 */
void EaglePlanJob_Delete(EaglePlanJob *job);

#endif
