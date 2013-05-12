#include <stdlib.h>
#include "EagleSynchronizer.h"
#include "EagleMemory.h"

EagleLock* EagleSynchronizer_CreateLock(void)
{
    EagleLock *lock = (EagleLock*) EagleMemory_Allocate("EagleSynchronizer_CreateLock.1", sizeof(EagleLock));
    if(NULL == lock) {
        return NULL;
    }
    
    pthread_mutex_init(&lock->mutex, NULL);
    return lock;
}

void EagleSynchronizer_Unlock(EagleLock *lock)
{
    pthread_mutex_unlock(&lock->mutex);
}
