#include <stdlib.h>
#include <stdio.h>
#include "EagleLinkedList.h"
#include "EagleMemory.h"

EagleLinkedListItem* EagleLinkedListItem_New(void *obj, EagleBoolean freeObj, void (*free)(void *obj))
{
    EagleLinkedListItem *item = (EagleLinkedListItem*) malloc(sizeof(EagleLinkedListItem));
    
    item->obj = obj;
    item->freeObj = freeObj;
    item->next = NULL;
    item->free = free;
    
    return item;
}

EagleLinkedList* EagleLinkedList_New(void)
{
    EagleLinkedList *list = (EagleLinkedList*) malloc(sizeof(EagleLinkedList));
    
    list->first = NULL;
    list->last = NULL;
    list->length = 0;
    list->modifyLock = EagleSynchronizer_CreateLock();
    
    return list;
}

void EagleLinkedList_add(EagleLinkedList *list, EagleLinkedListItem *item)
{
    if(NULL == item) {
        return;
    }
    
    EagleSynchronizer_Lock(list->modifyLock);
    if(NULL == list->last) {
        list->last = list->first = item;
    }
    else {
        EagleLinkedListItem *current = list->last;
        current->next = item;
        current = current->next;
        list->last = current;
    }
    ++list->length;
    EagleSynchronizer_Unlock(list->modifyLock);
}

int EagleLinkedList_length(EagleLinkedList *list)
{
    int length;
    EagleSynchronizer_Lock(list->modifyLock);
    length = list->length;
    EagleSynchronizer_Unlock(list->modifyLock);
    return length;
}

void EagleLinkedList_Delete(EagleLinkedList *list)
{
    EagleLinkedListItem *p, *next;
    EagleSynchronizer_Lock(list->modifyLock);
    
    for(p = list->first; NULL != p; p = next) {
        next = p->next;
        EagleLinkedListItem_Delete(p);
    }
    
    EagleSynchronizer_Unlock(list->modifyLock);
    EagleLock_Delete(list->modifyLock);
    EagleMemory_Free(list);
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

/**
 Return a pointer to the first item on the list.
 */
EagleLinkedListItem* EagleLinkedList_begin(EagleLinkedList *list)
{
    EagleLinkedListItem *first = NULL;
    EagleSynchronizer_Lock(list->modifyLock);
    first = list->first;
    EagleSynchronizer_Unlock(list->modifyLock);
    return first;
}

EagleLinkedListItem* EagleLinkedList_end(EagleLinkedList *list)
{
    EagleLinkedListItem *head = NULL;
    EagleSynchronizer_Lock(list->modifyLock);
    head = list->last;
    EagleSynchronizer_Unlock(list->modifyLock);
    return head;
}
