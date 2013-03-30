#include <string.h>
#include "EaglePageProviderArray.h"
#include "EagleUtils.h"
#include "EagleMemory.h"

EaglePageProviderArray* EaglePageProviderArray_NewInt(int *records, int totalRecords, int recordsPerPage, char *name)
{
    EaglePageProviderArray *pageProvider = (EaglePageProviderArray*) EagleMemory_Allocate("EaglePageProviderArray_NewInt.1", sizeof(EaglePageProviderArray));
    if(NULL == pageProvider) {
        return NULL;
    }
    
    pageProvider->providerType = EaglePageProviderTypeArray;
    pageProvider->type = EagleDataTypeInteger;
    pageProvider->name = (NULL == name ? NULL : strdup(name));
    pageProvider->recordsPerPage = recordsPerPage;
    
    pageProvider->offsetRecords = 0;
    pageProvider->totalRecords = totalRecords;
    pageProvider->records = records;
    pageProvider->nextPageLock = EagleSynchronizer_CreateLock();
    
    return pageProvider;
}

int EaglePageProviderArray_pagesRemaining(EaglePageProviderArray *epp)
{
    int r;
    
    EagleSynchronizer_Lock(epp->nextPageLock);
    r = EaglePageProvider_TotalPages(epp->totalRecords - epp->offsetRecords, epp->recordsPerPage);
    EagleSynchronizer_Unlock(epp->nextPageLock);
    
    return r;
}

void EaglePageProviderArray_Delete(EaglePageProviderArray *epp)
{
    if(NULL == epp) {
        return;
    }
    
    EagleLock_Delete(epp->nextPageLock);
    EagleMemory_Free(epp->name);
    EagleMemory_Free(epp);
}

EaglePage* EaglePageProviderArray_nextPage(EaglePageProviderArray *epp)
{
    int pageSize = MIN(epp->recordsPerPage, epp->totalRecords - epp->offsetRecords);
    EaglePage *page = NULL;
    
    if(epp->offsetRecords >= epp->totalRecords) {
        return NULL;
    }
    
    EagleSynchronizer_Lock(epp->nextPageLock);
    switch(epp->type) {
            
        case EagleDataTypeInteger:
        {
            EagleDataTypeIntegerType *begin = (EagleDataTypeIntegerType*) epp->records;
            page = EaglePage_New(epp->type, begin + epp->offsetRecords, pageSize, pageSize, epp->offsetRecords, EagleFalse);
            break;
        }
            
        case EagleDataTypeFloat:
        {
            EagleDataTypeFloatType *begin = (EagleDataTypeFloatType*) epp->records;
            page = EaglePage_New(epp->type, begin + epp->offsetRecords, pageSize, pageSize, epp->offsetRecords, EagleFalse);
            break;
        }
            
        case EagleDataTypeVarchar:
        {
            EagleDataTypeVarcharType *begin = (EagleDataTypeVarcharType*) epp->records;
            page = EaglePage_New(epp->type, begin + epp->offsetRecords, pageSize, pageSize, epp->offsetRecords, EagleFalse);
            break;
        }
            
        case EagleDataTypeUnknown:
            break;
            
    }
    epp->offsetRecords += pageSize;
    EagleSynchronizer_Unlock(epp->nextPageLock);
    
    return page;
}

void EaglePageProviderArray_reset(EaglePageProviderArray *epp)
{
    EagleSynchronizer_Lock(epp->nextPageLock);
    epp->offsetRecords = 0;
    EagleSynchronizer_Unlock(epp->nextPageLock);
}

EagleBoolean EaglePageProviderArray_isRandomAccess(EaglePageProviderArray *epp)
{
    return EagleTrue;
}

EaglePage* EaglePageProviderArray_getPage(EaglePageProviderArray *epp, int pageNumber)
{
    EaglePage *page = NULL;
    int offsetRecords = pageNumber * epp->recordsPerPage;
    
    /* check for out of range */
    if(pageNumber < 0 || pageNumber >= EaglePageProvider_TotalPages(epp->totalRecords, epp->recordsPerPage)) {
        return NULL;
    }
    
    switch(epp->type) {
            
        case EagleDataTypeInteger:
        {
            EagleDataTypeIntegerType *begin = (EagleDataTypeIntegerType*) epp->records;
            page = EaglePage_New(epp->type, begin + offsetRecords, epp->recordsPerPage, epp->recordsPerPage, offsetRecords, EagleFalse);
            break;
        }
            
        case EagleDataTypeFloat:
        {
            EagleDataTypeFloatType *begin = (EagleDataTypeFloatType*) epp->records;
            page = EaglePage_New(epp->type, begin + offsetRecords, epp->recordsPerPage, epp->recordsPerPage, offsetRecords, EagleFalse);
            break;
        }
            
        case EagleDataTypeVarchar:
        {
            EagleDataTypeVarcharType *begin = (EagleDataTypeVarcharType*) epp->records;
            page = EaglePage_New(epp->type, begin + offsetRecords, epp->recordsPerPage, epp->recordsPerPage, offsetRecords, EagleFalse);
            break;
        }
            
        case EagleDataTypeUnknown:
            break;
            
    }
    
    return page;
}
