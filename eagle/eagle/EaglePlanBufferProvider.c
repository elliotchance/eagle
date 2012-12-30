#include <stdlib.h>
#include <stdio.h>
#include "EaglePlanBufferProvider.h"

EaglePlanBufferProvider* EaglePlanBufferProvider_New(int destinationBuffer, EaglePageProvider *provider, EagleBoolean freeProvider)
{
    EaglePlanBufferProvider *bp = (EaglePlanBufferProvider*) malloc(sizeof(EaglePlanBufferProvider));
    
    bp->destinationBuffer = destinationBuffer;
    bp->provider = provider;
    bp->freeProvider = freeProvider;
    
    return bp;
}

char* EaglePlanBufferProvider_toString(EaglePlanBufferProvider *bp)
{
    char *msg = (char*) malloc(1024);
    sprintf(msg, "destination = %d, name = %s, type = %s", bp->destinationBuffer, bp->provider->name,
            EagleDataType_typeToName(bp->provider->type));
    return msg;
}

void EaglePlanBufferProvider_Delete(EaglePlanBufferProvider *bp)
{
    if(EagleTrue == bp->freeProvider) {
        EaglePageProvider_Delete(bp->provider);
    }
    free(bp);
}
