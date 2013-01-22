#ifndef eagle_EaglePlanJob_h
#define eagle_EaglePlanJob_h

#include "EaglePlan.h"

typedef struct {
    
    /**
     A reference to the plan.
     */
    EaglePlan *plan;
    
    /**
     The buffers.
     */
    EaglePage **buffers;
    
} EaglePlanJob;

EaglePlanJob* EaglePlanJob_New(EaglePlan *plan);

void EaglePlanJob_Delete(EaglePlanJob *job);

#endif
