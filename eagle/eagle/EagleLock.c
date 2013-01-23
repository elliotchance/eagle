#include "EagleLock.h"
#include "EagleMemory.h"

void EagleLock_Delete(EagleLock *lock)
{
    pthread_mutex_destroy(&lock->mutex);
    pthread_mutexattr_destroy(&lock->attr);
    EagleMemory_Free(lock);
}
