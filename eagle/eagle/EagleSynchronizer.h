#ifndef eagle_EagleSynchronizer_h
#define eagle_EagleSynchronizer_h

#include <pthread.h>

/**
 A lock is used to synchronize code blocks between threads.
 */
typedef struct {
    
    /**
     Internal mutex that holds the lock.
     */
    pthread_mutex_t mutex;
    
    /**
     Currently not in use, but it contains attributes for the internal mutex.
     */
    pthread_mutexattr_t attr;
    
} EagleLock;

EagleLock* EagleSynchronizer_CreateLock(void);
void EagleSynchronizer_Lock(EagleLock *lock);
void EagleSynchronizer_Unlock(EagleLock *lock);

void EagleLock_Delete(EagleLock *lock);

#endif
