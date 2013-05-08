#ifndef eagle_EagleSynchronizer_h
#define eagle_EagleSynchronizer_h

#include <mach/mach_time.h>
#include "EagleLock.h"
#include "EagleBoolean.h"

/**
 Create a new EagleLock.
 
 Locks can be used to synchronize block of code over multiple threads.
 
 @return A new lock instance.
 @see EagleSynchronizer_Lock()
 @see EagleSynchronizer_Unlock()
 */
EagleLock* EagleSynchronizer_CreateLock(void);

/**
 Lock an EagleLock.
 
 Only one thread may hold a lock at any given time so other threads trying to get access to this lock will wait until it
 is unlocked.
 
 @param [in] lock The lock initialized with EagleSynchronizer_CreateLock()
 
 @see EagleSynchronizer_Unlock()
 
uint64_t EagleSynchronizer_Lock(EagleLock *lock);*/

#define EagleSynchronizer_Lock(lock) { pthread_mutex_lock(&(lock)->mutex); }

/**
 Unlock an EagleLock.
 
 Only one thread may hold a lock at any given time so the next thread waiting for this lock will be able to continue.
 
 @param [in] lock The lock initialized with EagleSynchronizer_CreateLock()
 
 @see EagleSynchronizer_Lock()
 */
void EagleSynchronizer_Unlock(EagleLock *lock);

#endif
