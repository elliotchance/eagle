#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "EaglePageProvider.h"
#include "EaglePageOperations.h"
#include "EagleUtils.h"

EaglePageProvider* EaglePageProvider_New(int recordsPerPage)
{
    EaglePageProvider *pageProvider = (EaglePageProvider*) malloc(sizeof(EaglePageProvider));
    
    pageProvider->recordsPerPage = recordsPerPage;
    pageProvider->offsetRecords = 0;
    pageProvider->totalRecords = 0;
    pageProvider->records = NULL;
    pageProvider->nextPage = EaglePageProvider_nextPageFromStream_;
    pageProvider->pagesRemaining = EaglePageProvider_pagesRemainingFromStream_;
    pageProvider->obj = NULL;
    pageProvider->nextPageLock = EagleSynchronizer_CreateLock();
    
    return pageProvider;
}

EaglePageProvider* EaglePageProvider_CreateFromIntStream(int *records, int totalRecords, int recordsPerPage)
{
    EaglePageProvider *pageProvider = EaglePageProvider_New(recordsPerPage);
    
    pageProvider->records = records;
    pageProvider->totalRecords = totalRecords;
    
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
    EaglePageProvider *pageProvider = EaglePageProvider_CreateFromIntStream(data, recordsPerPage, recordsPerPage);
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

int EaglePageProvider_pagesRemainingFromStream_(EaglePageProvider *epp)
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

EaglePage* EaglePageProvider_nextPageFromStream_(EaglePageProvider *epp)
{
    int *begin = (int*) epp->records, *data = NULL;
    int pageSize = MIN(epp->recordsPerPage, epp->totalRecords - epp->offsetRecords);
    EaglePage *page;
    
    data = (int*) calloc((size_t) pageSize, sizeof(int));
    memmove((void*) data, (const void *) (begin + epp->offsetRecords), (unsigned long) (sizeof(int) * (size_t) pageSize));
    
    page = EaglePage_New(data, pageSize, epp->offsetRecords);
    epp->offsetRecords += pageSize;
    
    return page;
}

void EaglePageProvider_Delete(EaglePageProvider *epp)
{
    EagleLock_Delete(epp->nextPageLock);
    free(epp->obj);
    free(epp->records);
    free(epp);
}
