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

void EaglePageReceiver_pushInt(EaglePageReceiver *pr, int value)
{
    pr->buffer[pr->used++] = value;
}
