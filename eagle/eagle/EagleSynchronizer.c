#include <stdlib.h>
#include "EagleSynchronizer.h"

EagleLock* EagleSynchronizer_CreateLock(void)
{
    EagleLock *lock = (EagleLock*) malloc(sizeof(EagleLock));
    /* pthread_mutexattr_init(&lock->attr); */
    pthread_mutex_init(&lock->mutex, /* &lock->attr */ NULL);
    return lock;
}

void EagleSynchronizer_Lock(EagleLock *lock)
{
    pthread_mutex_lock(&lock->mutex);
}

void EagleSynchronizer_Unlock(EagleLock *lock)
{
    pthread_mutex_unlock(&lock->mutex);
}
