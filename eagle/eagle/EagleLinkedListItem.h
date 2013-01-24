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
    
};
typedef struct EagleLinkedListItem_ EagleLinkedListItem;

EagleLinkedListItem* EagleLinkedListItem_New(void *obj, EagleBoolean freeObj, void (*free)(void *obj));

void EagleLinkedListItem_Delete(EagleLinkedListItem *item);

#endif
