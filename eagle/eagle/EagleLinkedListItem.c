#include <stdlib.h>
#include "EagleLinkedListItem.h"
#include "EagleMemory.h"

EagleLinkedListItem* EagleLinkedListItem_New(void *obj, EagleBoolean freeObj, void (*free)(void *obj))
{
    EagleLinkedListItem *item = (EagleLinkedListItem*) EagleMemory_Allocate("EagleLinkedListItem_New.1", sizeof(EagleLinkedListItem));
    if(NULL == item) {
        return NULL;
    }
    
    item->obj = obj;
    item->freeObj = freeObj;
    item->next = NULL;
    item->free = free;
    
    return item;
}

void EagleLinkedListItem_Delete(EagleLinkedListItem *item)
{
    if(item->freeObj) {
        if(NULL == item->free) {
            EagleMemory_Free(item->obj);
        }
        else {
            item->free(item->obj);
        }
    }
    EagleMemory_Free(item);
}
