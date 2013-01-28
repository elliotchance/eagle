#ifndef eagle_EagleWorkers_h
#define eagle_EagleWorkers_h

#include "EagleWorker.h"

struct EagleInstance_;

/**
 A pool of workers.
 */
typedef struct {
    
    /**
     The workers in this pool.
     
     @note All workers are managed by this object, if you delete this object you will also kill all the of the workers.
     */
    EAGLE_ATTR_MANAGED EagleWorker **workers;
    
    /**
     The number of workers in this pool.
     */
    int totalWorkers;
    
} EagleWorkers;

/**
 Create a new pool of workers.
 @param [in] totalWorkers All the workers will be initialised (but not started) when the pool is created.
 @param [in] instance The eagle instance.
 @return New pool of workers.
 */
EagleWorkers* EagleWorkers_New(int totalWorkers, struct EagleInstance_ *instance);

/**
 * Delete a pool of workers.
 * @param [in] workers The pool.
 */
void EagleWorkers_Delete(EagleWorkers *workers);

/**
 * Start the pool of workers.
 * @param [in] workers The workers.
 */
void EagleWorkers_start(EagleWorkers *workers);

/**
 * Wait for all workers to finish. This is used when closing down the instance.
 * @param [in] workers The workers.
 */
void EagleWorkers_joinAll(EagleWorkers *workers);

#endif
