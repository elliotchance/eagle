#ifndef eagle_EaglePageReceiver_h
#define eagle_EaglePageReceiver_h

#include <pthread.h>
#include "EagleSynchronizer.h"

typedef struct {
    int allocated;
    int used;
    int *buffer;
    EagleLock *pushRecordIdLock;
} EaglePageReceiver;

EaglePageReceiver* EaglePageReceiver_New(void);
void EaglePageReceiver_Delete(EaglePageReceiver *receiver);

void EaglePageReceiver_pushRecordId(EaglePageReceiver *pr, int recordId);

#endif
