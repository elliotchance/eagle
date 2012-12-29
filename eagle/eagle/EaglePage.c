#include <stdlib.h>
#include <string.h>
#include "EaglePage.h"
#include "EagleUtils.h"

/**
 Create a new EaglePage.
 You will be required to provide a data block with the number of records in that data block.
 
 @param data The preallocated data block. This may be NULL, but the \p count must be 0.
 @param count The number of records in the data block.
 @param recordOffset An arbitrary number stored in the page. This number is used as an offset when calculating the
        record ID of a given record inside a page. If you are unsure as to what this value should be then you can use 0.
 @param freeData If EagleTrue the \p data will be freed with the page.
 */
EaglePage* EaglePage_New(EagleDataType type, void *data, int count, int recordOffset, EagleBoolean freeData)
{
    EaglePage *page = (EaglePage*) malloc(sizeof(EaglePage));
    
    page->type = type;
    page->data = data;
    page->count = count;
    page->recordOffset = recordOffset;
    page->freeData = freeData;
    
    return page;
}

/**
 Allocate a new page of given size.
 This function differs from EaglePage_New() because it will allocate the data block for you.
 
 @note The page allocated will not be filled with any data, make sure you sanitise the page if you intend to read from
       it.
 
 @param count The number of records of the page.
 */
EaglePage* EaglePage_AllocInt(int count)
{
    void *data = (void*) calloc((size_t) count, sizeof(int));
    return EaglePage_New(EagleDataTypeInteger, data, count, 0, EagleTrue);
}

EaglePage* EaglePage_AllocText(int count)
{
    void *data = (void*) calloc((size_t) count, sizeof(char*));
    return EaglePage_New(EagleDataTypeText, data, count, 0, EagleTrue);
}

void EaglePage_Delete(EaglePage *page)
{
    if(NULL == page) {
        return;
    }
    if(EagleTrue == page->freeData) {
        free((void*) page->data);
    }
    free((void*) page);
}

EaglePage* EaglePage_Copy(EaglePage *page)
{
    switch(page->type) {
            
        case EagleDataTypeUnknown:
            EagleUtils_Fatal("");
        
        case EagleDataTypeInteger:
            return EaglePage_CopyInt_(page);
            
        case EagleDataTypeText:
            return EaglePage_CopyText_(page);
            
    }
}

EaglePage* EaglePage_CopyInt_(EaglePage *page)
{
    size_t memorySize = (size_t) page->count * sizeof(int);
    int *newData = (int*) malloc(memorySize);
    
    memmove(newData, page->data, memorySize);
    return EaglePage_New(page->type, newData, page->count, page->recordOffset, page->freeData);
}

EaglePage* EaglePage_CopyText_(EaglePage *page)
{
    size_t memorySize = (size_t) page->count * sizeof(char*);
    void *newData = malloc(memorySize);
    
    memmove(newData, page->data, memorySize);
    return EaglePage_New(page->type, newData, page->count, page->recordOffset, page->freeData);
}

EaglePage* EaglePage_Alloc(EagleDataType type, int count)
{
    switch(type) {
            
        case EagleDataTypeUnknown:
            return NULL;
            
        case EagleDataTypeInteger:
            return EaglePage_AllocInt(count);
            
        case EagleDataTypeText:
            return EaglePage_AllocText(count);
            
    }
}
