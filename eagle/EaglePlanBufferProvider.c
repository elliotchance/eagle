#include <stdlib.h>
#include "EaglePlanBufferProvider.h"

EaglePlanBufferProvider* EaglePlanBufferProvider_New(int destinationBuffer, EaglePageProvider *provider)
{
    EaglePlanBufferProvider *bp = (EaglePlanBufferProvider*) malloc(sizeof(EaglePlanBufferProvider));
    bp->destinationBuffer = destinationBuffer;
    bp->provider = provider;
    return bp;
}
