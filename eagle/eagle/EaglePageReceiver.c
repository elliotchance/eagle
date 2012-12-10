#include <stdlib.h>
#include <stdio.h>
#include "EaglePageReceiver.h"

EaglePageReceiver* EaglePageReceiver_New(void)
{
    EaglePageReceiver *pr = (EaglePageReceiver*) malloc(sizeof(EaglePageReceiver));
    pr->allocated = 10000;
    pr->buffer = (int*) calloc((size_t) pr->allocated, sizeof(int));
    pr->used = 0;
    pr->pushRecordIdLock = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
    return pr;
}

void EaglePageReceiver_pushRecordId(EaglePageReceiver *pr, int recordId)
{
    /* synchronize this function */
    pthread_mutex_lock(pr->pushRecordIdLock);
    
    if(pr->used >= pr->allocated) {
        return;
    }
    pr->buffer[pr->used++] = recordId;
    
    pthread_mutex_unlock(pr->pushRecordIdLock);
}
