#ifndef eagle_EaglePageProviderArray_h
#define eagle_EaglePageProviderArray_h

#include "EaglePageProvider.h"

typedef struct {
    
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
 * Page remaining.
 * @param [in] epp The provider.
 * @return The number of pages remaining when the provider is made from an int array.
 */
int EaglePageProviderArray_pagesRemaining(EaglePageProviderArray *epp);

/**
 * Delete a provider.
 * @param [in] epp The provider.
 */
void EaglePageProviderArray_Delete(EaglePageProviderArray *epp);

EaglePage* EaglePageProviderArray_nextPage(EaglePageProviderArray *epp);

void EaglePageProviderArray_reset(EaglePageProviderArray *epp);

#endif
