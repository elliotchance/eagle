#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "EaglePageProvider.h"
#include "EaglePageOperations.h"
#include "EagleUtils.h"
#include "EagleLinkedList.h"
#include "EagleMemory.h"

EaglePageProvider* EaglePageProvider_CreateFromIntArray(int *records, int totalRecords, int recordsPerPage, char *name)
{
    EaglePageProvider *pageProvider = (EaglePageProvider*) EagleMemory_Allocate("EaglePageProvider_CreateFromIntArray.1", sizeof(EaglePageProvider));
    if(NULL == pageProvider) {
        return NULL;
    }
    
    pageProvider->recordsPerPage = recordsPerPage;
    pageProvider->offsetRecords = 0;
    pageProvider->totalRecords = totalRecords;
    pageProvider->records = records;
    pageProvider->nextPage = EaglePageProvider_nextPageFromIntArray_;
    pageProvider->pagesRemaining = EaglePageProvider_pagesRemainingFromIntArray_;
    pageProvider->add = EaglePageProvider_addUnsupported_;
    pageProvider->free = EaglePageProvider_DeleteIntArray_;
    pageProvider->nextPageLock = EagleSynchronizer_CreateLock();
    pageProvider->name = name;
    pageProvider->reset = EaglePageProvider_resetFromIntArray_;
    pageProvider->type = EagleDataTypeInteger;
    
    return pageProvider;
}

/**
 This creates a page provider that provides a single page filled with a fixed int.
 
 @param [in] value The value to fill the pages with.
 @param [in] recordsPerPage The number of records to return with each page.
 @param [in] name The name of the provider. Can contain any string, this may be a column name, an expression, etc.
 */
EaglePageProvider* EaglePageProvider_CreateFromInt(int value, int recordsPerPage, char *name)
{
    int *data = (int*) EagleMemory_MultiAllocate("EaglePageProvider_CreateFromInt.1", sizeof(int), recordsPerPage), i;
    EaglePageProvider *pageProvider;
    
    if(NULL == data) {
        return NULL;
    }
    
    pageProvider = EaglePageProvider_CreateFromIntArray(data, recordsPerPage, recordsPerPage, name);
    
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
    int pagesLeft;
    
    if(0 == totalRecords || 0 == recordsPerPage) {
        return 0;
    }
    
    pagesLeft = totalRecords / recordsPerPage;
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
    
    page = EaglePage_New(EagleDataTypeInteger, begin + epp->offsetRecords, pageSize, pageSize, epp->offsetRecords, EagleFalse);
    epp->offsetRecords += pageSize;
    
    return page;
}

void EaglePageProvider_Delete(EaglePageProvider *epp)
{
    if(NULL == epp) {
        return;
    }
    epp->free(epp);
    epp = NULL;
}

void EaglePageProvider_DeleteIntArray_(EaglePageProvider *epp)
{
    EagleLock_Delete(epp->nextPageLock);
    EagleMemory_Free(epp->records);
    EagleMemory_Free(epp);
}

void EaglePageProvider_DeleteStream_(EaglePageProvider *epp)
{
    EagleLock_Delete(epp->nextPageLock);
    EagleLinkedList_Delete((EagleLinkedList*) epp->records);
    EagleMemory_Free(epp->name);
    EagleMemory_Free(epp);
}

EagleBoolean EaglePageProvider_addUnsupported_(EaglePageProvider *epp, void *data)
{
    return EagleFalse;
}

EagleBoolean EaglePageProvider_addStream_(EaglePageProvider *epp, void *data)
{
    EagleLinkedList *list = (EagleLinkedList*) epp->records;
    EagleLinkedListItem *head = EagleLinkedList_end(list);
    EaglePage *page = NULL;
    
    if(NULL == head || ((EaglePage*) head->obj)->count >= epp->recordsPerPage) {
        /* the list is empty we need to create a new page */
        EagleLinkedListItem *item;
        page = EaglePage_Alloc(epp->type, epp->recordsPerPage);
        page->count = 0;
        item = EagleLinkedListItem_New(page, EagleTrue, (void (*)(void *obj)) EaglePage_Delete);
        EagleLinkedList_add(list, item);
    }
    else {
        page = (EaglePage*) head->obj;
    }
    
    switch(epp->type) {
            
        case EagleDataTypeUnknown:
            EagleUtils_Fatal("Unknown type.");
            
        case EagleDataTypeInteger:
            ((int*) page->data)[page->count++] = *((int*) data);
            break;
            
        case EagleDataTypeText:
            ((char**) page->data)[page->count++] = (char*) data;
            break;
        
    }
    
    ++epp->totalRecords;
    return EagleTrue;
}

EaglePageProvider* EaglePageProvider_CreateFromStream(EagleDataType type, int recordsPerPage, char *name)
{
    EaglePageProvider *pageProvider = (EaglePageProvider*) EagleMemory_Allocate("EaglePageProvider_CreateFromStream.1", sizeof(EaglePageProvider));
    if(NULL == pageProvider) {
        return NULL;
    }
    
    pageProvider->recordsPerPage = recordsPerPage;
    pageProvider->offsetRecords = 0;
    pageProvider->totalRecords = 0;
    pageProvider->records = (void*) EagleLinkedList_New();
    
    switch(type) {
            
        case EagleDataTypeUnknown:
            EagleUtils_Fatal("");
            
        case EagleDataTypeInteger:
            pageProvider->type = EagleDataTypeInteger;
            break;
            
        case EagleDataTypeText:
            pageProvider->type = EagleDataTypeText;
            break;
            
    }
    
    pageProvider->reset = EaglePageProvider_resetFromStream_;
    pageProvider->free = EaglePageProvider_DeleteStream_;
    pageProvider->nextPage = EaglePageProvider_nextPageFromStream_;
    pageProvider->pagesRemaining = EaglePageProvider_pagesRemainingFromStream_;
    pageProvider->add = EaglePageProvider_addStream_;
    pageProvider->nextPageLock = EagleSynchronizer_CreateLock();
    pageProvider->name = (NULL == name ? NULL : strdup(name));
    
    return pageProvider;
}

int EaglePageProvider_pagesRemainingFromStream_(EaglePageProvider *epp)
{
    return EaglePageProvider_TotalPages(epp->totalRecords - epp->offsetRecords, epp->recordsPerPage);
}

EaglePage* EaglePageProvider_nextPageFromStream_(EaglePageProvider *epp)
{
    int i = 0;
    EagleLinkedList *list = (EagleLinkedList*) epp->records;
    EagleLinkedListItem *begin;
    
    for(begin = EagleLinkedList_begin(list); begin != NULL; begin = begin->next) {
        if(i == epp->offsetRecords) {
            /* always give a duplicate page, so that the original page is not modified or freed */
            EaglePage *page = EaglePage_Copy((EaglePage*) begin->obj);
            page->recordOffset = epp->offsetRecords;
            epp->offsetRecords += page->count;
            return page;
        }
        i += ((EaglePage*) begin->obj)->count;
    }
    
    return NULL;
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

void EaglePageProvider_reset(EaglePageProvider *epp)
{
    /* synchronize this function */
    EagleSynchronizer_Lock(epp->nextPageLock);
    
    epp->reset(epp);
    
    EagleSynchronizer_Unlock(epp->nextPageLock);
}

void EaglePageProvider_resetFromIntArray_(EaglePageProvider *epp)
{
    epp->offsetRecords = 0;
}

void EaglePageProvider_resetFromStream_(EaglePageProvider *epp)
{
    epp->offsetRecords = 0;
}
