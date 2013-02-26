#ifndef eagle_EaglePageProviderSingle_h
#define eagle_EaglePageProviderSingle_h

#include "EaglePageProvider.h"

typedef struct {
    
    EaglePageProviderHeader;
    
    /**
     The value for the page.
     */
    EAGLE_ATTR_NA int value;
    
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
 * Free a page provider.
 * @param [in] epp Page provider instance.
 */
void EaglePageProviderSingle_Delete(EaglePageProviderSingle *epp);

int EaglePageProviderSingle_pagesRemaining(EaglePageProviderSingle *epp);

EaglePage* EaglePageProviderSingle_nextPage(EaglePageProviderSingle *epp);

void EaglePageProviderSingle_reset(EaglePageProviderSingle *epp);

#endif
