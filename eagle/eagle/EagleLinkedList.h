#ifndef eagle_EagleLinkedList_h
#define eagle_EagleLinkedList_h

#include "EagleSynchronizer.h"
#include "EagleBoolean.h"

struct EagleLinkedListItem_ {
    
    /**
     The actual item value.
     */
    void *obj;
    
    /**
     If EagleTrue the \c obj will be freed with the list item.
     */
    EagleBoolean freeObj;
    
    /**
     A pointer to the next item.
     */
    struct EagleLinkedListItem_ *next;
    
    /**
     A pointer to the function that will free \c obj. If this is NULL then the normal free() function will be used.
     */
    void (*free)(void *obj);
    
};
typedef struct EagleLinkedListItem_ EagleLinkedListItem;

typedef struct {
    
    /**
     A pointer to the last item.
     */
    EagleLinkedListItem *last;
    
    /**
     A pointer to the first item.
     */
    EagleLinkedListItem *first;
    
    /**
     The amount of items in this linked list.
     @see EagleLinkedList_length()
     */
    int length;
    
    /**
     Lock to synchronize list modifications.
     */
    EagleLock *modifyLock;
    
} EagleLinkedList;

EagleLinkedListItem* EagleLinkedListItem_New(void *obj, EagleBoolean freeObj, void (*free)(void *obj));

void EagleLinkedListItem_Delete(EagleLinkedListItem *item);

EagleLinkedList* EagleLinkedList_New(void);

void EagleLinkedList_Delete(EagleLinkedList *list);

void EagleLinkedList_add(EagleLinkedList *list, EagleLinkedListItem *item);

/**
 Return a pointer to the first item on the list.
 */
EagleLinkedListItem* EagleLinkedList_begin(EagleLinkedList *list);

EagleLinkedListItem* EagleLinkedList_end(EagleLinkedList *list);

int EagleLinkedList_length(EagleLinkedList *list);

#endif
