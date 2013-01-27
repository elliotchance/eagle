#include <stdlib.h>
#include <stdio.h>
#include "EagleLinkedList.h"
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

EagleLinkedList* EagleLinkedList_New(void)
{
    EagleLinkedList *list = (EagleLinkedList*) EagleMemory_Allocate("EagleLinkedList_New.1", sizeof(EagleLinkedList));
    if(NULL == list) {
        return NULL;
    }
    
    list->modifyLock = EagleSynchronizer_CreateLock();
    list->first = NULL;
    list->last = NULL;
    list->length = 0;
    
    return list;
}

void EagleLinkedList_add(EagleLinkedList *list, EagleLinkedListItem *item)
{
    if(NULL == list) {
        return;
    }
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
    
    if(NULL == list) {
        return 0;
    }
    
    EagleSynchronizer_Lock(list->modifyLock);
    length = list->length;
    EagleSynchronizer_Unlock(list->modifyLock);
    
    return length;
}

void EagleLinkedList_Delete(EagleLinkedList *list)
{
    if(NULL == list) {
        return;
    }
    
    EagleLock_Delete(list->modifyLock);
    EagleMemory_Free(list);
}

void EagleLinkedList_DeleteItems(EagleLinkedList *list)
{
    EagleLinkedListItem *p, *next;
    EagleSynchronizer_Lock(list->modifyLock);
    
    for(p = list->first; NULL != p; p = next) {
        next = p->next;
        EagleLinkedListItem_Delete(p);
    }
    
    EagleSynchronizer_Unlock(list->modifyLock);
}

void EagleLinkedList_DeleteWithItems(EagleLinkedList *list)
{
    if(NULL == list) {
        return;
    }
    
    EagleLinkedList_DeleteItems(list);
    EagleLinkedList_Delete(list);
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

EagleLinkedListItem* EagleLinkedList_pop(EagleLinkedList *list)
{
    EagleLinkedListItem *next;
    int i;
    
    if(EagleLinkedList_length(list) == 0) {
        return NULL;
    }
    if(EagleLinkedList_length(list) == 1) {
        EagleLinkedListItem *item = list->first;
        list->first = list->last = NULL;
        --list->length;
        return item;
    }
    
    /* find the second last item */
    next = list->first;
    for(i = 0; i < list->length - 2; ++i) {
        next = next->next;
    }
    
    --list->length;
    list->last = next;
    next->next->next = NULL;
    return next->next;
}

EagleBoolean EagleLinkedList_isEmpty(EagleLinkedList *list)
{
    if(NULL == list->first) {
        return EagleTrue;
    }
    return EagleFalse;
}

void** EagleLinkedList_toArray(EagleLinkedList *list, int *size)
{
    void **array;
    EagleLinkedListItem *p, *next;
    int i;
    
    *size = EagleLinkedList_length(list);
    array = (void**) EagleMemory_MultiAllocate("EagleLinkedList_toArray.1", sizeof(void*), *size);
    
    for(p = list->first, i = 0; NULL != p; p = next, ++i) {
        array[i] = p->obj;
        next = p->next;
    }
    
    return array;
}

void* EagleLinkedList_get(EagleLinkedList *list, int index)
{
    EagleLinkedListItem *next;
    int i;
    
    /* out of bounds */
    if(index < 0 || index >= EagleLinkedList_length(list)) {
        return NULL;
    }
    
    next = list->first;
    for(i = 0; i < index; ++i) {
        next = next->next;
    }
    
    return next->obj;
}

void EagleLinkedList_addObject(EagleLinkedList *list, void *obj, EagleBoolean freeObj, void (*free)(void *obj))
{
    EagleLinkedList_add(list, EagleLinkedListItem_New(obj, freeObj, free));
}
