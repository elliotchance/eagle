#ifndef eagle_EaglePageProviderArray_h
#define eagle_EaglePageProviderArray_h

#include "EaglePageProvider.h"

typedef struct {
    
    /**
     All "subtypes" of EaglePageProvider will have this header.
     */
    EaglePageProviderHeader;
    
    /**
     The cursor position. An internal counter of the position of the stream. If the stream has more pages this will be
     incremented with each nextPage().
     */
    EAGLE_ATTR_NA int offsetRecords;
    
    /**
     The total amount of records. You should never access this directly since its value may be virtual or invalid -
     instead use EaglePageProvider_pagesRemaining()
     */
    EAGLE_ATTR_NA int totalRecords;
    
    /**
     A pointer to the actual data that will be fed into pages. You should not rely on accessing this directly because
     its type and implementation will be different depending on the type of page provider. Use the appropriate methods
     nextPage() and pagesRemaining().
     
     The memory for this instance variable is only managed when the page provider is a stream type.
     */
    EAGLE_ATTR_SEMI_MANAGED void *records;
    
    /**
     Synchronize EaglePageProvider_nextPage() and EaglePageProvider_pagesRemaining()
     */
    EAGLE_ATTR_MANAGED EagleLock *nextPageLock;
    
} EaglePageProviderArray;

/**
 * Create a new read only page provider from a fixed size array.
 * @param [in] records The data records.
 * @param [in] totalRecords The total records.
 * @param [in] recordsPerPage The amount of records to serve out per page.
 * @param [in] name The name of the provider.
 * @return A new provider.
 */
EaglePageProviderArray* EaglePageProviderArray_NewInt(int *records, int totalRecords, int recordsPerPage, char *name);

/**
 * Pages remaining.
 * @param [in] epp The provider.
 * @return The number of pages remaining when the provider is made from an int array.
 */
int EaglePageProviderArray_pagesRemaining(EaglePageProviderArray *epp);

/**
 * Delete a provider.
 * @param [in] epp The provider.
 */
void EaglePageProviderArray_Delete(EaglePageProviderArray *epp);

/**
 * Get the next page from a provider.
 * @param [in] epp The provider.
 * @return The next page, or NULL.
 */
EaglePage* EaglePageProviderArray_nextPage(EaglePageProviderArray *epp);

/**
 * Reset the cursor of a provider.
 * @param [in] epp The provider.
 */
void EaglePageProviderArray_reset(EaglePageProviderArray *epp);

/**
 * Check is a providers pages can be access in any order with EaglePageProvider_getPage().
 * @param [in] epp The provider.
 */
EagleBoolean EaglePageProviderArray_isRandomAccess(EaglePageProviderArray *epp);

/**
 * Get a random page from a provider. You may only use this if EaglePageProvider_isRandomAccess() returns EagleTrue.
 * @param [in] epp The provider.
 * @param [in] pageNumber The number of the page where the first page will be 0.
 * @return The page, or NULL.
 */
EaglePage* EaglePageProviderArray_getPage(EaglePageProviderArray *epp, int pageNumber);

#endif
