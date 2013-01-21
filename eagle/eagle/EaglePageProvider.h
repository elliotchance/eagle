#ifndef eagle_EaglePageProvider_h
#define eagle_EaglePageProvider_h

#include <pthread.h>
#include "EaglePage.h"
#include "EagleSynchronizer.h"
#include "EagleBoolean.h"
#include "EagleLinkedList.h"

struct EaglePageProvider_ {
    
    /**
     The cursor position. An internal counter of the position of the stream. If the stream has more pages this will be
     incremented with each nextPage().
     */
    int offsetRecords;
    
    /**
     The total amount of records. You should never access this directly since its value may be virtual or invalid -
     instead use EaglePageProvider_pagesRemaining()
     */
    int totalRecords;
    
    /**
     For convenience the page provider has a default page size.
     */
    int recordsPerPage;
    
    /**
     A pointer to the actual data that will be fed into pages. You should not rely on accessing this directly because
     its type and implementation will be different depending on the type of page provider. Use the appropriate methods
     nextPage() and pagesRemaining().
     */
    void *records;
    
    /**
     Fetch the next page from the provider. If no more data is available this will return NULL.
     */
    EaglePage* (*nextPage)(struct EaglePageProvider_ *epp);
    
    /**
     Ask the provider if there is more data available. This must be invoked before every nextPage() - since the stream
     might be variable and more data may be added to the stream while your reading from it.
     */
    int (*pagesRemaining)(struct EaglePageProvider_ *epp);
    
    /**
     Some data provider types allow the addition of data.
     @return EagleTrue if the data was added on, EagleFalse otherwise.
     */
    EagleBoolean (*add)(struct EaglePageProvider_ *epp, void *data);
    
    /**
     Virtual method for freeing this structure.
     */
    void (*free)(struct EaglePageProvider_ *epp);
    
    /**
     Virtual method for resetting the page provider.
     */
    void (*reset)(struct EaglePageProvider_ *epp);
    
    /**
     Synchronize EaglePageProvider_nextPage() and EaglePageProvider_pagesRemaining()
     */
    EagleLock *nextPageLock;
    
    /**
     An optional name. For example, a column name. This is so named columns can be identified.
     */
    char *name;
    
    /**
     The data type for this provider.
     */
    EagleDataType type;
    
    /**
     This is used by streams. It allows it to keep the page it is upto when requesting the next page.
     */
    EagleLinkedListItem *cursor;
    
};
typedef struct EaglePageProvider_ EaglePageProvider;

EaglePageProvider* EaglePageProvider_CreateFromIntArray(int *records, int totalRecords, int recordsPerPage, char *name);
EaglePageProvider* EaglePageProvider_CreateFromInt(int value, int recordsPerPage, char *name);
EaglePageProvider* EaglePageProvider_CreateFromStream(EagleDataType type, int recordsPerPage, char *name);
int EaglePageProvider_TotalPages(int totalRecords, int recordsPerPage);
void EaglePageProvider_Delete(EaglePageProvider *epp);

int EaglePageProvider_pagesRemaining(EaglePageProvider *epp);
EaglePage* EaglePageProvider_nextPage(EaglePageProvider *epp);
EagleBoolean EaglePageProvider_add(EaglePageProvider *epp, void *data);
void EaglePageProvider_reset(EaglePageProvider *epp);

/* private functions */
EaglePageProvider* EaglePageProvider_New_(EagleDataType type, int recordsPerPage);
int EaglePageProvider_pagesRemainingFromIntArray_(EaglePageProvider *epp);
EaglePage* EaglePageProvider_nextPageFromIntArray_(EaglePageProvider *epp);
int EaglePageProvider_pagesRemainingFromInt_(EaglePageProvider *epp);
EaglePage* EaglePageProvider_nextPageFromInt_(EaglePageProvider *epp);
EagleBoolean EaglePageProvider_addUnsupported_(EaglePageProvider *epp, void *data);
EagleBoolean EaglePageProvider_addStream_(EaglePageProvider *epp, void *data);
int EaglePageProvider_pagesRemainingFromStream_(EaglePageProvider *epp);
EaglePage* EaglePageProvider_nextPageFromStream_(EaglePageProvider *epp);
void EaglePageProvider_DeleteIntArray_(EaglePageProvider *epp);
void EaglePageProvider_DeleteStream_(EaglePageProvider *epp);
void EaglePageProvider_resetFromIntArray_(EaglePageProvider *epp);
void EaglePageProvider_resetFromStream_(EaglePageProvider *epp);

#endif
