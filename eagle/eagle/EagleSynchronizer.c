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

/*uint64_t EagleSynchronizer_Lock(EagleLock *lock)
{
    uint64_t now = mach_absolute_time();
    pthread_mutex_lock(&lock->mutex);
    return mach_absolute_time() - now;
}*/

void EagleSynchronizer_Unlock(EagleLock *lock)
{
    pthread_mutex_unlock(&lock->mutex);
}
