#ifndef eagle_EagleSynchronizer_h
#define eagle_EagleSynchronizer_h

#include "EagleLock.h"

/**
 Create a new EagleLock.
 
 Locks can be used to synchronize block of code over multiple threads.
 
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
 */
void EagleSynchronizer_Lock(EagleLock *lock);

/**
 Unlock an EagleLock.
 
 Only one thread may hold a lock at any given time so the next thread waiting for this lock will be able to continue.
 
 @param [in] lock The lock initialized with EagleSynchronizer_CreateLock()
 
 @see EagleSynchronizer_Lock()
 */
void EagleSynchronizer_Unlock(EagleLock *lock);

#endif
