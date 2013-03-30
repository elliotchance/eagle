#ifndef eagle_EaglePageProviderVirtual_h
#define eagle_EaglePageProviderVirtual_h

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
     Object to be passed to the callback functions.
     */
    void *obj;
    
    /**
     Function pointer.
     */
    void (*free)(void*);
    
    /**
     Function pointer.
     */
    EagleBoolean (*add)(void*, void*);
    
    /**
     Function pointer.
     */
    int (*pagesRemaining)(void*);
    
    /**
     Function pointer.
     */
    EaglePage* (*nextPage)(void*);
    
    /**
     Function pointer.
     */
    void (*reset)(void*);
    
    /**
     Function pointer.
     */
    EaglePage* (*getPage)(void*, int);
    
} EaglePageProviderVirtual;

/**
 * Create a new writable page provider with zero records.
 * @param [in] recordsPerPage The amount of records per page.
 * @param [in] name The name of the provider.
 * @param [in] type Data type.
 * @param [in] obj This object is passed as the first argument to all callback functions.
 * @param [in] free Function pointer.
 * @param [in] add Function pointer.
 * @param [in] pagesRemaining Function pointer.
 * @param [in] nextPage Function pointer.
 * @param [in] reset Function pointer.
 * @param [in] getPage Function pointer. If this is NULL then EaglePageProvider_isRandomAccess() will return EagleFalse.
 * @return A new virtual provider.
 */
EaglePageProviderVirtual* EaglePageProviderVirtual_New(int recordsPerPage,
                                                       char *name,
                                                       EagleDataType type,
                                                       void *obj,
                                                       void (*free)(void*),
                                                       EagleBoolean (*add)(void*, void*),
                                                       int (*pagesRemaining)(void*),
                                                       EaglePage* (*nextPage)(void*),
                                                       void (*reset)(void*),
                                                       EaglePage* (*getPage)(void*, int));

/**
 * Free a page provider.
 * @param [in] epp Page provider instance.
 */
void EaglePageProviderVirtual_Delete(EaglePageProviderVirtual *epp);

/**
 * Add a record to a writable provider.
 * @param [in] epp The provider.
 * @param [in] data The data to add to the stream.
 * @return EagleTrue on success.
 */
EagleBoolean EaglePageProviderVirtual_add(EaglePageProviderVirtual *epp, void *data);

/**
 * Return the amount of pages remaining.
 * @param [in] epp The provider.
 * @return The number of pages available for reading.
 */
int EaglePageProviderVirtual_pagesRemaining(EaglePageProviderVirtual *epp);

/**
 * Get the next page.
 * @param [in] epp The provider.
 * @return The next page, or NULL.
 */
EaglePage* EaglePageProviderVirtual_nextPage(EaglePageProviderVirtual *epp);

/**
 * Reset the cursor of a provider.
 * @param [in] epp The provider.
 */
void EaglePageProviderVirtual_reset(EaglePageProviderVirtual *epp);

/**
 * Check is a providers pages can be access in any order with EaglePageProvider_getPage().
 * @param [in] epp The provider.
 */
EagleBoolean EaglePageProviderVirtual_isRandomAccess(EaglePageProviderVirtual *epp);

/**
 * Get a random page from a provider. You may only use this if EaglePageProvider_isRandomAccess() returns EagleTrue.
 * @param [in] epp The provider.
 * @param [in] pageNumber The number of the page where the first page will be 0.
 * @return The page, or NULL.
 */
EaglePage* EaglePageProviderVirtual_getPage(EaglePageProviderVirtual *epp, int pageNumber);

#endif
