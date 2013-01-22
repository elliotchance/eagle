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

/**
 Create a new eagle instance. The instance is effectivly a database, with a certain amount of workers (threads) that
 will carry out the work. Do not create multiple instances for the same database as there is no way to syncronise them.
 
 @param [in] totalWorkers The number of workers to launch, this does not include the main thread that will handle the
 connections and look after the workers.
 */
EagleInstance* EagleInstance_New(int totalWorkers);

/**
 Free EagleInstance
 
 @param [in] eagle Instance.
 */
void EagleInstance_Delete(EagleInstance *eagle);

/**
 Add an EaglePlan to an EagleInstance.
 
 @param [in] eagle Instance.
 @param [in] plan The plan to execute.
 */
void EagleInstance_addPlan(EagleInstance *eagle, EaglePlan *plan);

/**
 Run the instance. This will start it.
 
 @param [in] eagle Instance from EagleInstance_New()
 */
void EagleInstance_run(EagleInstance *eagle);

/**
 Get the next finite job from the stack. This is a syncronised method that all the workers will call independantly when
 they have nothing to do. If this method returns NULL it is upto the workers to know that there is no more work, they
 will check periodically after that.
 
 @param [in] eagle Instance.
 @return Initialised EaglePlanJob, or NULL if there are no available jobs.
 */
EaglePlanJob* EagleInstance_nextJob(EagleInstance *eagle);

#endif
