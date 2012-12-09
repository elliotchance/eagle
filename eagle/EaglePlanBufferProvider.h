#ifndef eagle_EaglePlanBufferProvider_h
#define eagle_EaglePlanBufferProvider_h

#include "EaglePageProvider.h"

typedef struct {
    int destinationBuffer;
    EaglePageProvider *provider;
} EaglePlanBufferProvider;

EaglePlanBufferProvider* EaglePlanBufferProvider_New(int destinationBuffer, EaglePageProvider *provider);

#endif
