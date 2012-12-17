#include <stdlib.h>
#include <stdio.h>
#include "EagleLinkedList.h"

EagleLinkedListItem* EagleLinkedListItem_New(void *obj, EagleBoolean freeObj)
{
    EagleLinkedListItem *item = (EagleLinkedListItem*) malloc(sizeof(EagleLinkedListItem));
    
    item->obj = obj;
    item->freeObj = freeObj;
    item->next = NULL;
    
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
    free(list);
    
    EagleSynchronizer_Unlock(list->modifyLock);
    EagleLock_Delete(list->modifyLock);
}

void EagleLinkedListItem_Delete(EagleLinkedListItem *item)
{
    if(item->freeObj) {
        free(item->obj);
    }
    free(item);
}

/**
 Return a pointer to the first item on the list.
 */
EagleLinkedListItem* EagleLinkedList_begin(EagleLinkedList *list)
{
    EagleLinkedListItem *head = NULL;
    EagleSynchronizer_Lock(list->modifyLock);
    head = list->first;
    EagleSynchronizer_Unlock(list->modifyLock);
    return head;
}
