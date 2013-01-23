#ifndef eagle_EaglePlanBufferProvider_h
#define eagle_EaglePlanBufferProvider_h

#include "EaglePageProvider.h"
#include "Eagle.h"

/**
 This structure acts as a link between a page providers data and the buffer for an expression it will be loaded into.
 */
typedef struct {
    
    /**
     The buffer ID where the next page will be loaded into.
     */
    EAGLE_ATTR_NA int destinationBuffer;
    
    /**
     The provider that contains the actual data for the buffer. This is semi managed because it depends on
     \c freeProvider
     */
    EAGLE_ATTR_SEMI_MANAGED EaglePageProvider *provider;
    
    /**
     Free the provider when the plan buffer provider is released.
     */
    EAGLE_ATTR_NA EagleBoolean freeProvider;
    
} EaglePlanBufferProvider;

EaglePlanBufferProvider* EaglePlanBufferProvider_New(int destinationBuffer, EaglePageProvider *provider, EagleBoolean freeProvider);

void EaglePlanBufferProvider_Delete(EaglePlanBufferProvider *bp);

char* EaglePlanBufferProvider_toString(EaglePlanBufferProvider *bp);

#endif
