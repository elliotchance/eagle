#include <stdlib.h>
#include <stdio.h>
#include "EaglePageReceiver.h"

EaglePageReceiver* EaglePageReceiver_New()
{
    EaglePageReceiver *pr = (EaglePageReceiver*) malloc(sizeof(EaglePageReceiver));
    pr->allocated = 10000;
    pr->buffer = (int*) calloc(sizeof(int), pr->allocated);
    pr->used = 0;
    return pr;
}

void EaglePageReceiver_pushRecordId(EaglePageReceiver *pr, int recordId)
{
    if(pr->used >= pr->allocated) {
        printf("receiver is out of memory!");
        return;
    }
    pr->buffer[pr->used++] = recordId;
}
