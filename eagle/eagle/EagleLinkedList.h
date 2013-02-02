#ifndef eagle_EagleLinkedList_h
#define eagle_EagleLinkedList_h

#include "EagleSynchronizer.h"
#include "EagleBoolean.h"
#include "Eagle.h"

struct EagleLinkedListItem_ {
    
    /**
     The actual item value.
     */
    EAGLE_ATTR_SEMI_MANAGED void *obj;
    
    /**
     If EagleTrue the \c obj will be freed with the list item.
     */
    EAGLE_ATTR_NA EagleBoolean freeObj;
    
    /**
     A pointer to the next item.
     */
    EAGLE_ATTR_PROVIDED struct EagleLinkedListItem_ *next;
    
    /**
     A pointer to the function that will free \c obj. If this is NULL then the normal free() function will be used.
     */
    EAGLE_ATTR_NA void (*free)(void *obj);
    
    /**
     A optional description of the item.
     */
    EAGLE_ATTR_PROVIDED char *description;
    
};
typedef struct EagleLinkedListItem_ EagleLinkedListItem;

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

EagleLinkedListItem* EagleLinkedListItem_New(void *obj, EagleBoolean freeObj, void (*free)(void *obj));

EagleLinkedListItem* EagleLinkedListItem_NewWithDescription(void *obj, EagleBoolean freeObj, void (*free)(void *obj), char *description);

void EagleLinkedListItem_Delete(EagleLinkedListItem *item);

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

EagleLinkedListItem* EagleLinkedList_pop(EagleLinkedList *list);

EagleBoolean EagleLinkedList_isEmpty(EagleLinkedList *list);

void EagleLinkedList_DeleteItems(EagleLinkedList *list);

/**
 Convert a linked list to an array. No data is duplicated so if the items are freed in the linked list they will
 disappear from your array as well.
 @param [in] list The linked list.
 @param [out] size The size of the array.
 @return An array of objects (no the linked list items, the actual objects)
 */
void** EagleLinkedList_toArray(EagleLinkedList *list, int *size);

void* EagleLinkedList_get(EagleLinkedList *list, int index);

void EagleLinkedList_addObject(EagleLinkedList *list, void *obj, EagleBoolean freeObj, void (*free)(void *obj));

EagleBoolean EagleLinkedList_deleteObject(EagleLinkedList *list, void *obj);

#endif
