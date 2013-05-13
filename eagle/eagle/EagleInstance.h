#ifndef eagle_EagleInstance_h
#define eagle_EagleInstance_h

#include <pthread.h>
#include "EagleWorkers.h"
#include "EaglePageProvider.h"
#include "EaglePlanJob.h"
#include "EagleSynchronizer.h"
#include "Eagle.h"

/**
 An eagle instance (a database).
 */
typedef struct EagleInstance {
    EAGLE_ATTR_MANAGED  EagleWorkers *workers;  /**< The workers. */
    EAGLE_ATTR_PROVIDED EaglePlan *plan;        /**< The plan that needs to be executed. */
    EAGLE_ATTR_MANAGED  EagleLock *nextJobLock; /**< To syncronise the EagleInstance_nextJob() method. */
} EagleInstance;

/**
 Create a new eagle instance. The instance is effectivly a database, with a certain amount of workers (threads) that
 will carry out the work. Do not create multiple instances for the same database as there is no way to syncronise them.
 
 @param [in] totalWorkers The number of workers to launch, this does not include the main thread that will handle the
 connections and look after the workers.
 @return New instance.
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
 @param [in] coreId CPU core ID.
 @return Initialised EaglePlanJob, or NULL if there are no available jobs.
 */
EaglePlanJob* EagleInstance_nextJob(EagleInstance *eagle, int coreId);

/**
 This is the internal non-synchronised version of EagleInstance_nextJob(). Having the core functionality in another
 function is mainly to help with profiling code.
 @param [in] plan The plan.
 @param [in] job A reference to the job.
 @param [in] coreId CPU core ID.
 */
void EagleInstance_nextJob_(EaglePlan *plan, EaglePlanJob **job, int coreId);

#endif
