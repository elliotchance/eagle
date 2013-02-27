#ifndef eagle_EaglePageProviderType_h
#define eagle_EaglePageProviderType_h

/**
 The internal type of a page provider.
 */
typedef enum {
    
    /**
     This page provider only contains a single value and will return pages filled with this value indefinitely.
     */
    EaglePageProviderTypeSingle = 1,
    
    /**
     A page provider that wraps an array of a fixed size.
     */
    EaglePageProviderTypeArray = 2,
    
    /**
     A read/write page provider that can be any length and can have records added to it.
     */
    EaglePageProviderTypeStream = 3,
    
    /**
     A virtual table does not contain any actual data, but rather relies on callback functions to get data when
     requested.
     */
    EaglePageProviderTypeVirtual = 4
    
} EaglePageProviderType;

#endif
