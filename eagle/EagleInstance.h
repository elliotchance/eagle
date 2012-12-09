#ifndef eagle_EagleInstance_h
#define eagle_EagleInstance_h

#include "EagleWorkers.h"
#include "EaglePageProvider.h"
#include "EaglePlan.h"

typedef struct {
    EagleWorkers *workers;
    EaglePlan *plan;
} EagleInstance;

EagleInstance* EagleInstance_New(int totalWorkers);

void EagleInstance_addPlan(EagleInstance *eagle, EaglePlan *plan);
void EagleInstance_run(EagleInstance *eagle);

#endif
