#include <stdlib.h>
#include <stdio.h>
#include "EaglePageReceiver.h"

EaglePageReceiver* EaglePageReceiver_New(void)
{
    EaglePageReceiver *pr = (EaglePageReceiver*) malloc(sizeof(EaglePageReceiver));
    pr->allocated = 10000;
    pr->buffer = (int*) calloc((size_t) pr->allocated, sizeof(int));
    pr->used = 0;
    pr->pushRecordIdLock = EagleSynchronizer_CreateLock();
    return pr;
}

void EaglePageReceiver_pushRecordId(EaglePageReceiver *pr, int recordId)
{
    /* synchronize this function */
    EagleSynchronizer_Lock(pr->pushRecordIdLock);
    
    if(pr->used >= pr->allocated) {
        EagleSynchronizer_Unlock(pr->pushRecordIdLock);
        return;
    }
    pr->buffer[pr->used++] = recordId;
    
    EagleSynchronizer_Unlock(pr->pushRecordIdLock);
}
