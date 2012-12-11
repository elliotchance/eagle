#ifndef eagle_EagleSynchronizer_h
#define eagle_EagleSynchronizer_h

#include <pthread.h>

typedef struct {
    pthread_mutex_t mutex;
    pthread_mutexattr_t attr;
} EagleLock;

EagleLock* EagleSynchronizer_CreateLock(void);
void EagleSynchronizer_Lock(EagleLock *lock);
void EagleSynchronizer_Unlock(EagleLock *lock);

#endif
