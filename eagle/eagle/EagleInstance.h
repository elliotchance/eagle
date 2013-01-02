#ifndef eagle_EagleInstance_h
#define eagle_EagleInstance_h

#include <pthread.h>
#include "EagleWorkers.h"
#include "EaglePageProvider.h"
#include "EaglePlanJob.h"
#include "EagleSynchronizer.h"

/**
 An eagle instance (a database).
 */
struct EagleInstance_ {
    EagleWorkers *workers;  /**< The workers. */
    EaglePlan *plan;        /**< The plan that needs to be executed. */
    EagleLock *nextJobLock; /**< To syncronise the EagleInstance_nextJob() method. */
};
typedef struct EagleInstance_ EagleInstance;

EagleInstance* EagleInstance_New(int totalWorkers);
void EagleInstance_Delete(EagleInstance *eagle);

void EagleInstance_addPlan(EagleInstance *eagle, EaglePlan *plan);
void EagleInstance_run(EagleInstance *eagle);
EaglePlanJob* EagleInstance_nextJob(EagleInstance *eagle);

#endif
