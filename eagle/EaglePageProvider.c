#include <stdlib.h>
#include <math.h>
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
    
    return pageProvider;
}

EaglePageProvider* EaglePageProvider_CreateFromIntStream(int *records, int totalRecords, int recordsPerPage)
{
    EaglePageProvider *pageProvider = EaglePageProvider_New(recordsPerPage);
    
    pageProvider->records = records;
    pageProvider->totalRecords = totalRecords;
    
    return pageProvider;
}

int EaglePageProvider_pagesRemaining(EaglePageProvider *epp)
{
    return epp->pagesRemaining(epp);
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
    return epp->nextPage(epp);
}

EaglePage* EaglePageProvider_nextPageFromStream_(EaglePageProvider *epp)
{
    int *begin = (int*) epp->records;
    int pageSize = MIN(epp->recordsPerPage, epp->totalRecords - epp->offsetRecords);
    
    EaglePage *page = EaglePage_New(begin + epp->offsetRecords, pageSize, epp->offsetRecords);
    epp->offsetRecords += pageSize;
    
    return page;
}