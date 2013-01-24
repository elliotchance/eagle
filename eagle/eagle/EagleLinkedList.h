#ifndef eagle_EagleLinkedList_h
#define eagle_EagleLinkedList_h

#include "EagleSynchronizer.h"
#include "EagleBoolean.h"
#include "Eagle.h"
#include "EagleLinkedListItem.h"

/**
 A linked list (FIFO).
 */
typedef struct {
    
    /**
     A pointer to the last item.
     */
    EAGLE_ATTR_PROVIDED EagleLinkedListItem *last;
    
    /**
     A pointer to the first item.
     */
    EAGLE_ATTR_PROVIDED EagleLinkedListItem *first;
    
    /**
     The amount of items in this linked list.
     @see EagleLinkedList_length()
     */
    EAGLE_ATTR_NA int length;
    
    /**
     Lock to synchronize list modifications.
     */
    EAGLE_ATTR_MANAGED EagleLock *modifyLock;
    
} EagleLinkedList;

EagleLinkedList* EagleLinkedList_New(void);

/**
 Delete a linked list.
 
 @note This will only remove the list, not the items contained in the list.
 
 @param [in] list The list object to free.
 @see EagleLinkedList_DeleteWithItems
 */
void EagleLinkedList_Delete(EagleLinkedList *list);

/**
 Delete a linked list and all the items contained in it.
 
 @param [in] list The list object to free.
 @see EagleLinkedList_Delete
 */
void EagleLinkedList_DeleteWithItems(EagleLinkedList *list);

void EagleLinkedList_add(EagleLinkedList *list, EagleLinkedListItem *item);

/**
 Return a pointer to the first item on the list.
 */
EagleLinkedListItem* EagleLinkedList_begin(EagleLinkedList *list);

/**
 Return a pointer to the last item on the list.
 */
EagleLinkedListItem* EagleLinkedList_end(EagleLinkedList *list);

int EagleLinkedList_length(EagleLinkedList *list);

#endif
