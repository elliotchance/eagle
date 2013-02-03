#include <stdlib.h>
#include <stdio.h>
#include "EagleLinkedList.h"
#include "EagleMemory.h"

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
    item->next = NULL;
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
    if(NULL == list) {
        return;
    }
    if(EagleTrue == EagleLinkedList_isEmpty(list)) {
        return;
    }
    
    {
        EagleLinkedListItem *p, *next;
        EagleSynchronizer_Lock(list->modifyLock);
        
        for(p = list->first; NULL != p; p = next) {
            next = p->next;
            EagleLinkedListItem_Delete(p);
        }
        list->first = list->last = NULL;
        list->length = 0;
        
        EagleSynchronizer_Unlock(list->modifyLock);
    }
}

void EagleLinkedList_DeleteWithItems(EagleLinkedList *list)
{
    if(NULL == list) {
        return;
    }
    
    EagleLinkedList_DeleteItems(list);
    EagleLinkedList_Delete(list);
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
    EagleLinkedListItem *next, *r;
    int i;
    
    if(EagleLinkedList_length(list) == 0) {
        return NULL;
    }
    if(EagleLinkedList_length(list) == 1) {
        EagleLinkedListItem *item = list->first;
        list->first = list->last = NULL;
        --list->length;
        item->next = NULL;
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
    r = next->next;
    list->last->next = NULL;
    return r;
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
    
    if(NULL == list) {
        return NULL;
    }
    
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

EagleBoolean EagleLinkedList_deleteObject(EagleLinkedList *list, void *obj)
{
    EagleLinkedListItem *last, *cursor;
    
    /* problems */
    if(NULL == obj) {
        return EagleFalse;
    }
    if(EagleTrue == EagleLinkedList_isEmpty(list)) {
        return EagleFalse;
    }
    
    /* one item */
    if(EagleLinkedList_length(list) == 1) {
        if(list->first->obj == obj) {
            EagleLinkedListItem_Delete(list->first);
            list->length = 0;
            list->first = NULL;
            list->last = NULL;
            
            return EagleTrue;
        }
        return EagleFalse;
    }
    
    /* begin item */
    if(EagleLinkedList_begin(list)->obj == obj) {
        EagleLinkedListItem *second = list->first->next;
        
        EagleLinkedListItem_Delete(list->first);
        --list->length;
        list->first = second;
        
        return EagleTrue;
    }
    
    /* some item after the first */
    last = EagleLinkedList_begin(list);
    for(cursor = last->next; NULL != cursor; cursor = cursor->next, last = last->next) {
        if(obj == cursor->obj) {
            /* if this is the last item we need to fix the last pointer */
            if(list->last == cursor) {
                list->last = last;
            }
            
            last->next = cursor->next;
            EagleLinkedListItem_Delete(cursor);
            --list->length;
            return EagleTrue;
        }
    }
    
    /* object not found on list */
    return EagleFalse;
}
