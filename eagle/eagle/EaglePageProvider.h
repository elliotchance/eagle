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

#define EaglePageProviderHeader EaglePageProviderType providerType; EagleDataType type; char *name; int recordsPerPage

/**
 A page provider.
 */
typedef struct {
    
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

EagleBoolean EaglePageProvider_add(EaglePageProvider *epp, void *data);

int EaglePageProvider_pagesRemaining(EaglePageProvider *epp);

EaglePage* EaglePageProvider_nextPage(EaglePageProvider *epp);

#endif
