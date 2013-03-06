#ifndef eagle_EaglePageProviderSingle_h
#define eagle_EaglePageProviderSingle_h

#include "EaglePageProvider.h"

typedef struct {
    
    /**
     All "subtypes" of EaglePageProvider will have this header.
     */
    EaglePageProviderHeader;
    
    /**
     Contains the value.
     */
    union {
        
        /**
         Integer value.
         */
        EAGLE_ATTR_NA int intValue;
        
        /**
         String value.
         */
        EAGLE_ATTR_MANAGED const char *strValue;
        
    } value;
    
} EaglePageProviderSingle;

/**
 * This creates a page provider that provides a single page filled with a fixed int.
 * @param [in] value The value to fill the pages with.
 * @param [in] recordsPerPage The number of records to return with each page.
 * @param [in] name The name of the provider. Can contain any string, this may be a column name, an expression, etc.
 * @return A new provider.
 */
EaglePageProviderSingle* EaglePageProviderSingle_NewInt(int value, int recordsPerPage, char *name);

/**
 * This creates a page provider that provides a single page filled with a fixed string.
 * @param [in] value The value to fill the pages with.
 * @param [in] recordsPerPage The number of records to return with each page.
 * @param [in] name The name of the provider. Can contain any string, this may be a column name, an expression, etc.
 * @return A new provider.
 */
EaglePageProviderSingle* EaglePageProviderSingle_NewVarchar(const char *value, int recordsPerPage, char *name);

/**
 * Free a page provider.
 * @param [in] epp Page provider instance.
 */
void EaglePageProviderSingle_Delete(EaglePageProviderSingle *epp);

/**
 * Return the amount of pages remaining.
 * @param [in] epp The provider.
 * @return The number of pages available for reading.
 */
int EaglePageProviderSingle_pagesRemaining(EaglePageProviderSingle *epp);

/**
 * Get the next page from a provider.
 * @param [in] epp The provider.
 * @return The next page, or NULL.
 */
EaglePage* EaglePageProviderSingle_nextPage(EaglePageProviderSingle *epp);

/**
 * Reset the cursor of a provider.
 * @param [in] epp The provider.
 */
void EaglePageProviderSingle_reset(EaglePageProviderSingle *epp);

#endif
