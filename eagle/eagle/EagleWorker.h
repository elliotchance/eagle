#ifndef eagle_EagleWorker_h
#define eagle_EagleWorker_h

#include <pthread.h>
#include "Eagle.h"
#include "EaglePlanJob.h"

struct EagleInstance_;

/**
 A worker is a thread.
 */
typedef struct {
    
    /**
     A unique ID for the worker thread.
     */
    EAGLE_ATTR_NA int workerId;
    
    /**
     The actual thread this worker represents.
     */
    EAGLE_ATTR_NA pthread_t thread;
    
    /**
     The instance this worker belongs to.
     */
    EAGLE_ATTR_PROVIDED struct EagleInstance_ *instance;
    
} EagleWorker;

/**
 * Create a new worker (thread)
 * @param [in] workerId The worker ID is just an arbitrary number that should be unique to the worker for logging purposes.
 * @param [in] instance The instance the workers belong to.
 * @return A new worker.
 */
EagleWorker* EagleWorker_New(int workerId, struct EagleInstance_ *instance);

/**
 * Delete a worker.
 * @param [in] worker The worker.
 */
void EagleWorker_Delete(EagleWorker *worker);

/**
 * Start a worker.
 * @param [in] worker The worker.
 */
void EagleWorker_start(EagleWorker *worker);

/**
 * Begin a worker.
 * @param [in] obj This is a EagleWorker* but it must have void* to fit the definition for the pthread.
 * @return Ignored.
 */
void* EagleWorker_begin(void *obj);

/**
 * Wait for a worker to complete and shutdown.
 * @param [in] worker The worker.
 */
void EagleWorker_join(EagleWorker *worker);

/**
 Execute a job.
 @param [in] job The job to execute.
 */
void EagleWorker_runJob(EaglePlanJob *job);

/**
 Internal function used by EagleWorker_runJob().
 @param [in] job The job to execute.
 @param [in] epo The job plan operation.
 */
void EagleWorker_runJobLiteral_(EaglePlanJob *job, EaglePlanOperation *epo);

/**
 Internal function used by EagleWorker_runJob().
 @param [in] job The job to execute.
 @param [in] epo The job plan operation.
 */
void EagleWorker_runJobPage_(EaglePlanJob *job, EaglePlanOperation *epo);

#endif
