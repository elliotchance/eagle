#ifndef eagle_EaglePlanJob_h
#define eagle_EaglePlanJob_h

#include "EaglePlan.h"

typedef struct {
    EaglePlan *plan;
    int allocatedBuffers;
    EaglePage **buffers;
} EaglePlanJob;

EaglePlanJob* EaglePlanJob_New(EaglePlan *plan, int allocatedBuffers);
void EaglePlanJob_Delete(EaglePlanJob *job);

#endif
