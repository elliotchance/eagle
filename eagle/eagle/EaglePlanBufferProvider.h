#ifndef eagle_EaglePlanBufferProvider_h
#define eagle_EaglePlanBufferProvider_h

#include "EaglePageProvider.h"
#include "Eagle.h"
#include "EagleDbSqlValue.h"

/**
 A buffer provider can be a container for a traditional generic page provider or for a single value.
 */
typedef enum {
    
    EaglePlanBufferProviderTypeProvider = 1, /**< This buffer provider wraps a generic page provider. */
    
    EaglePlanBufferProviderTypeValue = 2     /**< This buffer provider wraps a EagleDbSqlValue */
    
} EaglePlanBufferProviderType;

/**
 This structure acts as a link between a page providers data and the buffer for an expression it will be loaded into.
 */
typedef struct {
    
    /**
     The buffer ID where the next page will be loaded into.
     */
    EAGLE_ATTR_NA int destinationBuffer;
    
    /**
     The provider type.
     */
    EAGLE_ATTR_NA EaglePlanBufferProviderType type;
    
    /**
     The value.
     */
    union {
        
        /**
         Value for a provider.
         */
        struct {
            
            /**
             The provider that contains the actual data for the buffer. This is semi managed because it depends on
             \c freeProvider
             */
            EAGLE_ATTR_SEMI_MANAGED EaglePageProvider *provider;
            
            /**
             Free the provider when the plan buffer provider is released.
             */
            EAGLE_ATTR_NA EagleBoolean freeProvider;
            
        } provider;
        
        /**
         Fixed single value.
         */
        EAGLE_ATTR_PROVIDED EagleDbSqlValue *value;
        
    } value;
    
} EaglePlanBufferProvider;

/**
 * Create a new plan buffer provider with a generic provider.
 * @param [in] destinationBuffer The destination buffer ID.
 * @param [in] provider The provider.
 * @param [in] freeProvider Free the \p provider when this instance is deleted?
 * @return A new plan buffer provider.
 */
EaglePlanBufferProvider* EaglePlanBufferProvider_NewWithProvider(int destinationBuffer,
                                                                 EaglePageProvider *provider,
                                                                 EagleBoolean freeProvider);

/**
 * Create a new plan buffer provider with a EagleDbSqlValue.
 * @param [in] destinationBuffer The destination buffer ID.
 * @param [in] value The value.
 * @return A new plan buffer provider.
 */
EaglePlanBufferProvider* EaglePlanBufferProvider_NewWithValue(int destinationBuffer,
                                                              EagleDbSqlValue *value);

/**
 * Delete a plan buffer provider.
 * @param [in] bp The instance.
 */
void EaglePlanBufferProvider_Delete(EaglePlanBufferProvider *bp);

/**
 * Get the description for a plan buffer provider. This will return a new string that you will have to free when you're
 * finished with it.
 * @param [in] bp The instance.
 * @return A new string description.
 */
char* EaglePlanBufferProvider_toString(EaglePlanBufferProvider *bp);

#endif
