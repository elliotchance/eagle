#include <stdlib.h>
#include "EagleLinkedListItem.h"
#include "EagleMemory.h"

EagleLinkedListItem* EagleLinkedListItem_New(void *obj, EagleBoolean freeObj, void (*free)(void *obj))
{
    return EagleLinkedListItem_NewWithDescription(obj, freeObj, free, NULL);
}

EagleLinkedListItem* EagleLinkedListItem_NewWithDescription(void *obj, EagleBoolean freeObj, void (*free)(void *obj), char *description)
{
    EagleLinkedListItem *item = (EagleLinkedListItem*) EagleMemory_Allocate("EagleLinkedListItem_NewWithDescription.1", sizeof(EagleLinkedListItem));
    if(NULL == item) {
        return NULL;
    }
    
    item->obj = obj;
    item->freeObj = freeObj;
    item->next = NULL;
    item->free = free;
    item->description = (NULL == description ? NULL : description);
    
    return item;
}

void EagleLinkedListItem_Delete(EagleLinkedListItem *item)
{
    if(NULL == item) {
        return;
    }
    
    if(item->freeObj) {
        if(NULL == item->free) {
            EagleMemory_Free(item->obj);
        }
        else {
            item->free(item->obj);
        }
    }
    EagleMemory_Free(item->description);
    EagleMemory_Free(item);
}
