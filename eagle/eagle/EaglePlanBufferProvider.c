#include <stdlib.h>
#include <stdio.h>
#include "EaglePlanBufferProvider.h"

EaglePlanBufferProvider* EaglePlanBufferProvider_New(int destinationBuffer, EaglePageProvider *provider)
{
    EaglePlanBufferProvider *bp = (EaglePlanBufferProvider*) malloc(sizeof(EaglePlanBufferProvider));
    
    bp->destinationBuffer = destinationBuffer;
    bp->provider = provider;
    
    return bp;
}

char* EaglePlanBufferProvider_toString(EaglePlanBufferProvider *bp)
{
    char *msg = (char*) malloc(1024);
    sprintf(msg, "destination = %d", bp->destinationBuffer);
    return msg;
}

void EaglePlanBufferProvider_Delete(EaglePlanBufferProvider *bp)
{
    EaglePageProvider_Delete(bp->provider);
    free(bp);
}
