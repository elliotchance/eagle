#ifndef eagle_EagleWorker_h
#define eagle_EagleWorker_h

#include <pthread.h>
#include "Eagle.h"

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

EagleWorker* EagleWorker_New(int workerId, struct EagleInstance_ *instance);

void EagleWorker_Delete(EagleWorker *worker);

void EagleWorker_start(EagleWorker *worker);

void* EagleWorker_begin(void *obj);

void EagleWorker_join(EagleWorker *worker);

#endif
