#ifndef eagle_EagleInstance_h
#define eagle_EagleInstance_h

#include <pthread.h>
#include "EagleWorkers.h"
#include "EaglePageProvider.h"
#include "EaglePlanJob.h"
#include "EagleSynchronizer.h"

struct EagleInstance_ {
    EagleWorkers *workers;
    EaglePlan *plan;
    EagleLock *nextJobLock;
};
typedef struct EagleInstance_ EagleInstance;

EagleInstance* EagleInstance_New(int totalWorkers);
void EagleInstance_Delete(EagleInstance *eagle);

void EagleInstance_addPlan(EagleInstance *eagle, EaglePlan *plan);
void EagleInstance_run(EagleInstance *eagle);
EaglePlanJob* EagleInstance_nextJob(EagleInstance *eagle);

#endif
