#ifndef eagle_EaglePageProviderStream_h
#define eagle_EaglePageProviderStream_h

#include "EagleDataType.h"
#include "EagleBoolean.h"
#include "EaglePage.h"
#include "EaglePageProvider.h"

typedef struct {
    
    /**
     All "subtypes" of EaglePageProvider will have this header.
     */
    EaglePageProviderHeader;
    
    /**
     This is used by streams. It allows it to keep the page it is upto when requesting the next page.
     */
    EAGLE_ATTR_PROVIDED EagleLinkedListItem *cursor;
    
    /**
     A list containing EaglePage's with the records.
     */
    EAGLE_ATTR_MANAGED EagleLinkedList *list;
    
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
     Synchronize EaglePageProvider_nextPage() and EaglePageProvider_pagesRemaining()
     */
    EAGLE_ATTR_MANAGED EagleLock *nextPageLock;
    
} EaglePageProviderStream;

/**
 * Create a new writable page provider with zero records.
 * @param [in] type The data type for the provider (and its pages).
 * @param [in] recordsPerPage The amount of records per page.
 * @param [in] name The name of the provider.
 * @return A new read/write provider.
 */
EaglePageProviderStream* EaglePageProviderStream_New(EagleDataType type, int recordsPerPage, char *name);

/**
 * Free a page provider.
 * @param [in] epp Page provider instance.
 */
void EaglePageProviderStream_Delete(EaglePageProviderStream *epp);

/**
 * Add a record to a writable provider.
 * @param [in] epp The provider.
 * @param [in] data The data to add to the stream.
 * @return EagleTrue on success.
 */
EagleBoolean EaglePageProviderStream_add(EaglePageProviderStream *epp, void *data);

/**
 * Return the amount of pages remaining at the moment for a read/write provider.
 * @param [in] epp The provider.
 * @return The number of pages available for reading.
 */
int EaglePageProviderStream_pagesRemaining(EaglePageProviderStream *epp);

/**
 * Get the next page from a stream provider.
 * @param [in] epp The provider.
 * @return The next page, or NULL.
 */
EaglePage* EaglePageProviderStream_nextPage(EaglePageProviderStream *epp);

/**
 * Reset the cursor of a provider.
 * @param [in] epp The provider.
 */
void EaglePageProviderStream_reset(EaglePageProviderStream *epp);

/**
 * Check is a providers pages can be access in any order with EaglePageProvider_getPage().
 * @param [in] epp The provider.
 */
EagleBoolean EaglePageProviderStream_isRandomAccess(EaglePageProviderStream *epp);

/**
 * Get a random page from a provider. You may only use this if EaglePageProvider_isRandomAccess() returns EagleTrue.
 * @param [in] epp The provider.
 * @param [in] pageNumber The number of the page where the first page will be 0.
 * @return The page, or NULL.
 */
EaglePage* EaglePageProviderStream_getPage(EaglePageProviderStream *epp, int pageNumber);

#endif
