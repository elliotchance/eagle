#ifndef eagle_EagleLinkedList_h
#define eagle_EagleLinkedList_h

#include "EagleSynchronizer.h"
#include "EagleBoolean.h"
#include "Eagle.h"
#include "EagleLinkedListItem.h"

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

/**
 * Append an item to a linked list.
 * @param [in] list The list.
 * @param [in] item The item.
 */
void EagleLinkedList_add(EagleLinkedList *list, EagleLinkedListItem *item);

/**
 * Return a pointer to the first item on the list.
 * @param [in] list The list.
 * @return The pointer to the first element in the list.
 */
EagleLinkedListItem* EagleLinkedList_begin(EagleLinkedList *list);

/**
 * Return a pointer to the last item on the list.
 * @param [in] list The list.
 * @return The pointer to the last item in the list.
 */
EagleLinkedListItem* EagleLinkedList_end(EagleLinkedList *list);

/**
 * The number of items in the list.
 * @param [in] list The list.
 * @return The number of items in the list.
 */
int EagleLinkedList_length(EagleLinkedList *list);

/**
 * Remove the last item on the list and return it. Since this does not free the item you will have to free it yourself.
 * @param list The list to pop from.
 * @see EagleLinkedListItem_Delete()
 * @return The linked list item on the end of the list.
 */
EagleLinkedListItem* EagleLinkedList_pop(EagleLinkedList *list);

/**
 * Check if the list is empty.
 * @param list The list.
 * @return EagleTrue is there are no items on the list.
 */
EagleBoolean EagleLinkedList_isEmpty(EagleLinkedList *list);

/**
 * Delete the items (but not the list itself). This is useful for clearing out a list and reusing it.
 * @param list The list.
 */
void EagleLinkedList_DeleteItems(EagleLinkedList *list);

/**
 Convert a linked list to an array. No data is duplicated so if the items are freed in the linked list they will
 disappear from your array as well.
 @param [in] list The linked list.
 @param [out] size The size of the array.
 @return An array of objects (no the linked list items, the actual objects)
 */
void** EagleLinkedList_toArray(EagleLinkedList *list, int *size);

/**
 * Get an item by index. The first item will have the index of 0.
 * @param list The list.
 * @param index The index.
 * @return The object is returned (not the linked list item). If the index is out of bounds then NULL will be returned.
 */
void* EagleLinkedList_get(EagleLinkedList *list, int index);

/**
 * Add an object to the list. This is an easier method then creating the linked list item first, but it means you have
 * to free the items with the list.
 * @param list The list.
 * @param obj The object.
 * @param freeObj Free the object when the item is freed.
 * @param free The function responsible for freeing the objec. This may be NULL if you want to use EagleMemory_Free()
 */
void EagleLinkedList_addObject(EagleLinkedList *list, void *obj, EagleBoolean freeObj, void (*free)(void *obj));

EagleBoolean EagleLinkedList_deleteObject(EagleLinkedList *list, void *obj);

#endif
