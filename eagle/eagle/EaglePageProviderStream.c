#include <string.h>
#include "EaglePageProviderStream.h"
#include "EagleMemory.h"
#include "EagleLogger.h"

EaglePageProviderStream* EaglePageProviderStream_New(EagleDataType type, int recordsPerPage, char *name)
{
    EaglePageProviderStream *pageProvider = (EaglePageProviderStream*) EagleMemory_Allocate("EaglePageProviderStream_New.1", sizeof(EaglePageProviderStream));
    if(NULL == pageProvider) {
        return NULL;
    }
    
    switch(type) {
            
        case EagleDataTypeUnknown:
            EagleMemory_Free(pageProvider);
            EagleLogger_Log(EagleLoggerSeverityError, "Unknown type.");
            return NULL;
            
        case EagleDataTypeInteger:
            pageProvider->type = EagleDataTypeInteger;
            break;
            
        case EagleDataTypeVarchar:
            pageProvider->type = EagleDataTypeVarchar;
            break;
            
        case EagleDataTypeFloat:
            pageProvider->type = EagleDataTypeFloat;
            break;
            
    }
    
    pageProvider->providerType = EaglePageProviderTypeStream;
    pageProvider->recordsPerPage = recordsPerPage;
    pageProvider->offsetRecords = 0;
    pageProvider->totalRecords = 0;
    pageProvider->list = EagleLinkedList_New();
    pageProvider->nextPageLock = EagleSynchronizer_CreateLock();
    pageProvider->name = (NULL == name ? NULL : strdup(name));
    pageProvider->cursor = NULL;
    
    return pageProvider;
}

void EaglePageProviderStream_Delete(EaglePageProviderStream *epp)
{
    if(NULL == epp) {
        return;
    }
    
    EagleLock_Delete(epp->nextPageLock);
    EagleLinkedList_DeleteWithItems(epp->list);
    EagleMemory_Free(epp->name);
    EagleMemory_Free(epp);
}

EagleBoolean EaglePageProviderStream_add(EaglePageProviderStream *epp, void *data)
{
    EagleLinkedListItem *head;
    EaglePage *page = NULL;
    
    EagleSynchronizer_Lock(epp->nextPageLock);
    
    head = EagleLinkedList_end(epp->list);
    if(NULL == head || ((EaglePage*) head->obj)->count >= epp->recordsPerPage) {
        /* the list is empty we need to create a new page */
        EagleLinkedListItem *item;
        page = EaglePage_Alloc(epp->type, epp->recordsPerPage);
        page->count = 0;
        item = EagleLinkedListItem_New(page, EagleTrue, (void (*)(void *obj)) EaglePage_Delete);
        EagleLinkedList_add(epp->list, item);
    }
    else {
        page = (EaglePage*) head->obj;
    }
    
    switch(epp->type) {
            
        case EagleDataTypeUnknown:
            EagleLogger_Log(EagleLoggerSeverityError, "Unknown type.");
            EagleSynchronizer_Unlock(epp->nextPageLock);
            return EagleFalse;
            
        case EagleDataTypeInteger:
            ((EagleDataTypeIntegerType*) page->data)[page->count++] = *((EagleDataTypeIntegerType*) data);
            break;
            
        case EagleDataTypeVarchar:
            ((EagleDataTypeVarcharType*) page->data)[page->count++] = (EagleDataTypeVarcharType) data;
            break;
            
        case EagleDataTypeFloat:
            ((EagleDataTypeFloatType*) page->data)[page->count++] = *((EagleDataTypeFloatType*) data);
            break;
            
    }
    
    ++epp->totalRecords;
    EagleSynchronizer_Unlock(epp->nextPageLock);
    
    return EagleTrue;
}

int EaglePageProviderStream_pagesRemaining(EaglePageProviderStream *epp)
{
    int r;
    
    EagleSynchronizer_Lock(epp->nextPageLock);
    r = EaglePageProvider_TotalPages(epp->totalRecords - epp->offsetRecords, epp->recordsPerPage);
    EagleSynchronizer_Unlock(epp->nextPageLock);
    
    return r;
}

EaglePage* EaglePageProviderStream_nextPage(EaglePageProviderStream *epp)
{
    EaglePage *page;
    
    EagleSynchronizer_Lock(epp->nextPageLock);
    
    if(NULL == epp->cursor) {
        epp->cursor = EagleLinkedList_begin(epp->list);
    }
    
    /* there is no more pages to give */
    if(epp->offsetRecords >= epp->totalRecords || NULL == epp->cursor) {
        EagleSynchronizer_Unlock(epp->nextPageLock);
        return NULL;
    }
    
    /* always give a duplicate page, so that the original page is not modified or freed */
    page = EaglePage_Copy((EaglePage*) epp->cursor->obj);
    page->recordOffset = epp->offsetRecords;
    epp->offsetRecords += page->count;
    epp->cursor = epp->cursor->next;
    
    EagleSynchronizer_Unlock(epp->nextPageLock);
    return page;
}

void EaglePageProviderStream_reset(EaglePageProviderStream *epp)
{
    epp->offsetRecords = 0;
    epp->cursor = NULL;
}

EagleBoolean EaglePageProviderStream_isRandomAccess(EaglePageProviderStream *epp)
{
    return EagleFalse;
}

EaglePage* EaglePageProviderStream_getPage(EaglePageProviderStream *epp, int pageNumber)
{
    return NULL;
}
