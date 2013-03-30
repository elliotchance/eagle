#ifndef eagle_EaglePageProvider_h
#define eagle_EaglePageProvider_h

#include <pthread.h>
#include "EaglePage.h"
#include "EagleSynchronizer.h"
#include "EagleBoolean.h"
#include "EagleLinkedList.h"
#include "Eagle.h"
#include "EagleDataType.h"
#include "EaglePageProviderType.h"

/**
 All "subtypes" of EaglePageProvider will have this header.
 */
#define EaglePageProviderHeader EaglePageProviderType providerType; EagleDataType type; char *name; int recordsPerPage

/**
 A page provider.
 */
typedef struct {
    
    /**
     All "subtypes" of EaglePageProvider will have this header.
     */
    EaglePageProviderHeader;
    
} EaglePageProvider;

/**
 * Calculate the amount of pages required to serve a given amount of records.
 * @param [in] totalRecords The total records.
 * @param [in] recordsPerPage The maximum records a single page can hold.
 * @return The number of pages left.
 */
int EaglePageProvider_TotalPages(int totalRecords, int recordsPerPage);

/**
 * Reset the cursor of a provider.
 * @param [in] epp The provider.
 */
void EaglePageProvider_reset(EaglePageProvider *epp);

/**
 * Delete a provider.
 * @param [in] epp The provider.
 */
void EaglePageProvider_Delete(EaglePageProvider *epp);

/**
 * Add a record to a writable provider (if possible).
 * @param [in] epp The provider.
 * @param [in] data The data to add to the stream.
 * @return EagleTrue on success.
 */
EagleBoolean EaglePageProvider_add(EaglePageProvider *epp, void *data);

/**
 * Return the amount of pages remaining.
 * @param [in] epp The provider.
 * @return The number of pages available for reading.
 */
int EaglePageProvider_pagesRemaining(EaglePageProvider *epp);

/**
 * Get the next page from a provider.
 * @param [in] epp The provider.
 * @return The next page, or NULL.
 */
EaglePage* EaglePageProvider_nextPage(EaglePageProvider *epp);

/**
 * Check is a providers pages can be access in any order with EaglePageProvider_getPage().
 * @param [in] epp The provider.
 */
EagleBoolean EaglePageProvider_isRandomAccess(EaglePageProvider *epp);

/**
 * Get a random page from a provider. You may only use this if EaglePageProvider_isRandomAccess() returns EagleTrue.
 * @param [in] epp The provider.
 * @param [in] pageNumber The number of the page where the first page will be 0.
 * @return The page, or NULL.
 */
EaglePage* EaglePageProvider_getPage(EaglePageProvider *epp, int pageNumber);

#endif
