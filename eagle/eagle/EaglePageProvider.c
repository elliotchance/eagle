#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "EaglePageProvider.h"
#include "EaglePageOperations.h"
#include "EagleUtils.h"

EaglePageProvider* EaglePageProvider_CreateFromIntArray(int *records, int totalRecords, int recordsPerPage)
{
    EaglePageProvider *pageProvider = (EaglePageProvider*) malloc(sizeof(EaglePageProvider));
    
    pageProvider->recordsPerPage = recordsPerPage;
    pageProvider->offsetRecords = 0;
    pageProvider->totalRecords = totalRecords;
    pageProvider->records = records;
    pageProvider->nextPage = EaglePageProvider_nextPageFromIntArray_;
    pageProvider->pagesRemaining = EaglePageProvider_pagesRemainingFromIntArray_;
    pageProvider->add = EaglePageProvider_addUnsupported_;
    pageProvider->nextPageLock = EagleSynchronizer_CreateLock();
    
    return pageProvider;
}

/**
 This creates a page provider that provides a single page filled with a fixed int.
 
 @param value The value to fill the pages with.
 @param recordsPerPage The number of records to return with each page.
 */
EaglePageProvider* EaglePageProvider_CreateFromInt(int value, int recordsPerPage)
{
    int *data = (int*) calloc((size_t) recordsPerPage, sizeof(int)), i;
    EaglePageProvider *pageProvider = EaglePageProvider_CreateFromIntArray(data, recordsPerPage, recordsPerPage);
    for(i = 0; i < recordsPerPage; ++i) {
        data[i] = value;
    }
    return pageProvider;
}

int EaglePageProvider_pagesRemaining(EaglePageProvider *epp)
{
    int pagesRemaining;
    
    /* synchronize this function */
    EagleSynchronizer_Lock(epp->nextPageLock);
    
    pagesRemaining = epp->pagesRemaining(epp);
    
    EagleSynchronizer_Unlock(epp->nextPageLock);
    return pagesRemaining;
}

int EaglePageProvider_TotalPages(int totalRecords, int recordsPerPage)
{
    int pagesLeft = totalRecords / recordsPerPage;
    if(totalRecords % recordsPerPage > 0) {
        ++pagesLeft;
    }
    return pagesLeft;
}

int EaglePageProvider_pagesRemainingFromIntArray_(EaglePageProvider *epp)
{
    return EaglePageProvider_TotalPages(epp->totalRecords - epp->offsetRecords, epp->recordsPerPage);
}

EaglePage* EaglePageProvider_nextPage(EaglePageProvider *epp)
{
    EaglePage *nextPage;
    
    /* synchronize this function */
    EagleSynchronizer_Lock(epp->nextPageLock);
    
    nextPage = epp->nextPage(epp);
    
    EagleSynchronizer_Unlock(epp->nextPageLock);
    return nextPage;
}

EaglePage* EaglePageProvider_nextPageFromIntArray_(EaglePageProvider *epp)
{
    int *begin = (int*) epp->records;
    int pageSize = MIN(epp->recordsPerPage, epp->totalRecords - epp->offsetRecords);
    EaglePage *page;
    
    page = EaglePage_New(begin + epp->offsetRecords, pageSize, epp->offsetRecords, EagleFalse);
    epp->offsetRecords += pageSize;
    
    return page;
}

void EaglePageProvider_Delete(EaglePageProvider *epp)
{
    if(!epp) {
        return;
    }
    EagleLock_Delete(epp->nextPageLock);
    free(epp->records);
    free(epp);
}

EagleBoolean EaglePageProvider_addUnsupported_(EaglePageProvider *epp, void *data)
{
    return EagleFalse;
}

EagleBoolean EaglePageProvider_addStream_(EaglePageProvider *epp, void *data)
{
    ((int*) epp->records)[epp->totalRecords++] = *((int*) data);
    return EagleTrue;
}

EaglePageProvider* EaglePageProvider_CreateFromIntStream(int recordsPerPage)
{
    EaglePageProvider *pageProvider = (EaglePageProvider*) malloc(sizeof(EaglePageProvider));
    
    pageProvider->recordsPerPage = recordsPerPage;
    pageProvider->offsetRecords = 0;
    pageProvider->totalRecords = 0;
    pageProvider->records = (int*) calloc((size_t) 1000, sizeof(int));
    pageProvider->nextPage = EaglePageProvider_nextPageFromIntStream_;
    pageProvider->pagesRemaining = EaglePageProvider_pagesRemainingFromIntStream_;
    pageProvider->add = EaglePageProvider_addStream_;
    pageProvider->nextPageLock = EagleSynchronizer_CreateLock();
    
    return pageProvider;
}

int EaglePageProvider_pagesRemainingFromIntStream_(EaglePageProvider *epp)
{
    return EaglePageProvider_TotalPages(epp->totalRecords - epp->offsetRecords, epp->recordsPerPage);
}

EaglePage* EaglePageProvider_nextPageFromIntStream_(EaglePageProvider *epp)
{
    int *begin = (int*) epp->records, *data = NULL;
    int pageSize = MIN(epp->recordsPerPage, epp->totalRecords - epp->offsetRecords);
    EaglePage *page;
    
    data = (int*) calloc((size_t) pageSize, sizeof(int));
    memmove((void*) data, (const void *) (begin + epp->offsetRecords), (unsigned long) (sizeof(int) * (size_t) pageSize));
    
    page = EaglePage_New(data, pageSize, epp->offsetRecords, EagleTrue);
    epp->offsetRecords += pageSize;
    
    return page;
}

EagleBoolean EaglePageProvider_add(EaglePageProvider *epp, void *data)
{
    EagleBoolean r;
    
    /* synchronize this function */
    EagleSynchronizer_Lock(epp->nextPageLock);
    
    r = epp->add(epp, data);
    
    EagleSynchronizer_Unlock(epp->nextPageLock);
    return r;
}
