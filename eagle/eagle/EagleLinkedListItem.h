#ifndef eagle_EagleLinkedListItem_h
#define eagle_EagleLinkedListItem_h

#include "Eagle.h"
#include "EagleBoolean.h"

/**
 A linked list item.
 A linked list can only contain instances of EagleLinkedListItem.
 */
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
    EAGLE_ATTR_MANAGED char *description;
    
};
typedef struct EagleLinkedListItem_ EagleLinkedListItem;

/**
 * Initialise a new item to append to a linked list.
 * @param [in] obj The object.
 * @param [in] freeObj Free the object when linked list item is freed.
 * @param [in] free The free function.
 * @return A new linked list item.
 */
EagleLinkedListItem* EagleLinkedListItem_New(void *obj, EagleBoolean freeObj, void (*free)(void *obj));

/**
 * Free a linked list item.
 * @param [in] item The item.
 */
void EagleLinkedListItem_Delete(EagleLinkedListItem *item);

/**
 * Create a new linked list item with an attached description. The description is often useful for debugging.
 * @param [in] obj The object.
 * @param [in] freeObj Free the object when linked list item is freed.
 * @param [in] free The free function.
 * @param [in] description A description.
 * @return A new linked list item.
 */
EagleLinkedListItem* EagleLinkedListItem_NewWithDescription(void *obj, EagleBoolean freeObj, void (*free)(void *obj), char *description);

#endif
