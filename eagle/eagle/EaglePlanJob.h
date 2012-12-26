#ifndef eagle_EaglePlanJob_h
#define eagle_EaglePlanJob_h

#include "EaglePlan.h"

typedef struct {
    
    /**
     A reference to the plan.
     */
    EaglePlan *plan;
    
    /**
     The total number of allocated buffers.
     */
    int allocatedBuffers;
    
    /**
     The buffers.
     */
    EaglePage **buffers;
    
} EaglePlanJob;

EaglePlanJob* EaglePlanJob_New(EaglePlan *plan, int allocatedBuffers);
void EaglePlanJob_Delete(EaglePlanJob *job);

#endif
