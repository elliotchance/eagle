#include <stdlib.h>
#include <stdio.h>
#include "EaglePlanBufferProvider.h"
#include "EagleMemory.h"

EaglePlanBufferProvider* EaglePlanBufferProvider_New(int destinationBuffer, EaglePageProvider *provider, EagleBoolean freeProvider)
{
    EaglePlanBufferProvider *bp = (EaglePlanBufferProvider*) EagleMemory_Allocate("EaglePlanBufferProvider_New.1", sizeof(EaglePlanBufferProvider));
    if(NULL == bp) {
        return NULL;
    }
    
    bp->destinationBuffer = destinationBuffer;
    bp->provider = provider;
    bp->freeProvider = freeProvider;
    
    return bp;
}

char* EaglePlanBufferProvider_toString(EaglePlanBufferProvider *bp)
{
    char *msg = (char*) EagleMemory_Allocate("EaglePlanBufferProvider_toString.1", 1024), *name;
    if(NULL == msg) {
        return NULL;
    }
    
    name = EagleDataType_typeToName(bp->provider->type);
    sprintf(msg, "destination = %d, name = %s, type = %s", bp->destinationBuffer, bp->provider->name, name);
    EagleMemory_Free(name);
    
    return msg;
}

void EaglePlanBufferProvider_Delete(EaglePlanBufferProvider *bp)
{
    if(NULL == bp) {
        return;
    }
    
    if(EagleTrue == bp->freeProvider) {
        EaglePageProvider_Delete(bp->provider);
    }
    EagleMemory_Free(bp);
}
