#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "EaglePage.h"
#include "EagleUtils.h"
#include "EagleMemory.h"
#include "EagleLogger.h"

EaglePage* EaglePage_New(EagleDataType type, void *data, int totalSize, int count, int recordOffset, EagleBoolean freeData)
{
    EaglePage *page = (EaglePage*) EagleMemory_Allocate("EaglePage_New.1", sizeof(EaglePage));
    if(NULL == page) {
        return NULL;
    }
    
    page->type = type;
    page->data = data;
    page->totalSize = totalSize;
    page->count = count;
    page->recordOffset = recordOffset;
    page->freeData = freeData;
    page->usageCount = 1;
    
    return page;
}

EaglePage* EaglePage_AllocInt(int count)
{
    void *data = (void*) EagleMemory_MultiAllocate("EaglePage_AllocInt.1", sizeof(EagleDataTypeIntegerType), count);
    if(NULL == data) {
        return NULL;
    }
    
    return EaglePage_New(EagleDataTypeInteger, data, count, count, 0, EagleTrue);
}

EaglePage* EaglePage_AllocFloat(int count)
{
    void *data = (void*) EagleMemory_MultiAllocate("EaglePage_AllocFloat.1", sizeof(EagleDataTypeFloatType), count);
    if(NULL == data) {
        return NULL;
    }
    
    return EaglePage_New(EagleDataTypeFloat, data, count, count, 0, EagleTrue);
}

EaglePage* EaglePage_AllocVarchar(int count)
{
    void *data = (void*) EagleMemory_MultiAllocate("EaglePage_AllocVarchar.1", sizeof(EagleDataTypeVarcharType), count);
    if(NULL == data) {
        return NULL;
    }
    
    return EaglePage_New(EagleDataTypeVarchar, data, count, 0, 0, EagleTrue);
}

void EaglePage_Delete(EaglePage *page)
{
    if(NULL == page) {
        return;
    }
    
    /* decrement the usage count */
    --page->usageCount;
    
    /* if this object is still in use then we can't free it yet */
    if(page->usageCount > 0) {
        return;
    }
    
    if(EagleTrue == page->freeData) {
        switch(page->type) {
                
            case EagleDataTypeInteger:
            case EagleDataTypeUnknown:
            case EagleDataTypeFloat:
                break;
                
            case EagleDataTypeVarchar:
            {
                /* free all strings first */
                int i;
                
                for(i = 0; i < page->count; ++i) {
                    EagleMemory_Free(((EagleDataTypeVarcharType*) page->data)[i]);
                }
                break;
            }
                
        }
        
        EagleMemory_Free((void*) page->data);
    }
    
    EagleMemory_Free((void*) page);
}

EaglePage* EaglePage_Copy(EaglePage *page)
{
    /* increment the usage count but return the same page */
    ++page->usageCount;
    return page;
}

EaglePage* EaglePage_RealCopy(EaglePage *page)
{
    switch(page->type) {
            
        case EagleDataTypeUnknown:
            EagleLogger_Log(EagleLoggerSeverityError, "Cannot page of Unknown type.");
            return NULL;
            
        case EagleDataTypeInteger:
            return EaglePage_RealCopyInt_(page);
            
        case EagleDataTypeVarchar:
            return EaglePage_RealCopyVarchar_(page);
            
        case EagleDataTypeFloat:
            return EaglePage_RealCopyFloat_(page);
            
    }
}

EaglePage* EaglePage_RealCopyInt_(EaglePage *page)
{
    size_t memorySize;
    EagleDataTypeIntegerType *newData;
    
    if(NULL == page) {
        return NULL;
    }
    
    memorySize = (size_t) page->count * sizeof(EagleDataTypeIntegerType);
    newData = (EagleDataTypeIntegerType*) EagleMemory_Allocate("EaglePage_CopyInt_.1", memorySize);
    if(NULL == newData) {
        return NULL;
    }
    
    memmove(newData, page->data, memorySize);
    return EaglePage_New(page->type, newData, page->totalSize, page->count, page->recordOffset, page->freeData);
}

EaglePage* EaglePage_RealCopyFloat_(EaglePage *page)
{
    size_t memorySize;
    EagleDataTypeFloatType *newData;
    
    if(NULL == page) {
        return NULL;
    }
    
    memorySize = (size_t) page->count * sizeof(EagleDataTypeFloatType);
    newData = (EagleDataTypeFloatType*) EagleMemory_Allocate("EaglePage_CopyFloat_.1", memorySize);
    if(NULL == newData) {
        return NULL;
    }
    
    memmove(newData, page->data, memorySize);
    return EaglePage_New(page->type, newData, page->totalSize, page->count, page->recordOffset, page->freeData);
}

EaglePage* EaglePage_RealCopyVarchar_(EaglePage *page)
{
    EagleDataTypeVarcharType *newData;
    int i;
    
    if(NULL == page) {
        return NULL;
    }
    
    newData = (EagleDataTypeVarcharType*) EagleMemory_MultiAllocate("EaglePage_CopyVarchar_.1", sizeof(EagleDataTypeVarcharType), page->count);
    if(NULL == newData) {
        return NULL;
    }
    
    for(i = 0; i < page->count; ++i) {
        newData[i] = strdup(((EagleDataTypeVarcharType*) page->data)[i]);
    }
    
    return EaglePage_New(page->type, newData, page->totalSize, page->count, page->recordOffset, page->freeData);
}

EaglePage* EaglePage_Alloc(EagleDataType type, int count)
{
    switch(type) {
            
        case EagleDataTypeUnknown:
            return NULL;
            
        case EagleDataTypeInteger:
            return EaglePage_AllocInt(count);
            
        case EagleDataTypeVarchar:
            return EaglePage_AllocVarchar(count);
            
        case EagleDataTypeFloat:
            return EaglePage_AllocFloat(count);
            
    }
}

char* EaglePage_toString(EaglePage *page)
{
    char* buf = (char*) EagleMemory_Allocate("EaglePage_toString.1", 8192), *type;
    if(NULL == buf) {
        return NULL;
    }
    
    type = EagleDataType_typeToName(page->type);
    sprintf(buf, "EaglePage { type = %s, size = %d, count = %d, offset = %d }", type, page->totalSize, page->count,
            page->recordOffset);
    EagleMemory_Free(type);
    return buf;
}
