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

EagleWorkers* EagleWorkers_New(int totalWorkers, struct EagleInstance_ *instance);

void EagleWorkers_Delete(EagleWorkers *workers);

void EagleWorkers_start(EagleWorkers *workers);

void EagleWorkers_joinAll(EagleWorkers *workers);

#endif
