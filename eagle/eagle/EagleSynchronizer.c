#include <stdlib.h>
#include "EagleSynchronizer.h"

/**
 Create a new EagleLock.
 
 Locks can be used to synchronize block of code over multiple threads.
 
 @see EagleSynchronizer_Lock()
 @see EagleSynchronizer_Unlock()
 */
EagleLock* EagleSynchronizer_CreateLock(void)
{
    EagleLock *lock = (EagleLock*) malloc(sizeof(EagleLock));
    pthread_mutex_init(&lock->mutex, NULL);
    return lock;
}

/**
 Lock an EagleLock.
 
 Only one thread may hold a lock at any given time so other threads trying to get access to this lock will wait until it
 is unlocked.
 
 @param lock The lock initialized with EagleSynchronizer_CreateLock()
 
 @see EagleSynchronizer_Unlock()
 */
void EagleSynchronizer_Lock(EagleLock *lock)
{
    pthread_mutex_lock(&lock->mutex);
}

/**
 Unlock an EagleLock.
 
 Only one thread may hold a lock at any given time so the next thread waiting for this lock will be able to continue.
 
 @param lock The lock initialized with EagleSynchronizer_CreateLock()
 
 @see EagleSynchronizer_Lock()
 */
void EagleSynchronizer_Unlock(EagleLock *lock)
{
    pthread_mutex_unlock(&lock->mutex);
}

void EagleLock_Delete(EagleLock *lock)
{
    pthread_mutex_destroy(&lock->mutex);
    pthread_mutexattr_destroy(&lock->attr);
    free(lock);
}
