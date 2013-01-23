#ifndef eagle_EagleLock_h
#define eagle_EagleLock_h

#include <pthread.h>
#include "Eagle.h"

/**
 A lock is used to synchronize code blocks between threads.
 */
typedef struct {
    
    /**
     Internal mutex that holds the lock.
     */
    EAGLE_ATTR_MANAGED pthread_mutex_t mutex;
    
    /**
     Currently not in use, but it contains attributes for the internal mutex.
     */
    EAGLE_ATTR_MANAGED pthread_mutexattr_t attr;
    
} EagleLock;

void EagleLock_Delete(EagleLock *lock);

#endif
