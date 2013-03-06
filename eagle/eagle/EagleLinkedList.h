#ifndef eagle_EagleLinkedList_h
#define eagle_EagleLinkedList_h

#include "EagleSynchronizer.h"
#include "EagleBoolean.h"
#include "Eagle.h"
#include "EagleLinkedListItem.h"

/**
 Iterate through a linked list. This is safe no nest as long as the third parameter \p _name is unique to each nested
 loop.
 
 @param [in] _list The EagleLinkedList.
 @param [in] _type The type for each individual EagleLinkedListItem.
 @param [in] _name The name of the iterator containing your object.
 
 Example
 @code
 EagleLinkedList_Foreach(db->schemas, EagleDbSchema*, schema)
 {
     char *name = schema->name;
 
     // Also exposes:
     //   _cursor_schema   The EagleLinkedListItem
     //   _i_schema        The item count (starting at 0)
 }
 EagleLinkedList_ForeachEnd
 @endcode
 */
#define EagleLinkedList_Foreach(_list, _type, _name) { \
EagleLinkedListItem *_cursor##_##_name; \
int _i##_##_name; \
for(_cursor##_##_name = EagleLinkedList_begin(_list), _i##_##_name = 0; NULL != _cursor##_##_name; _cursor##_##_name = _cursor##_##_name->next, ++_i##_##_name) { \
_type _name = (_type) _cursor##_##_name->obj;

/**
 This MUST always follow a EagleLinkedList_ForeachEnd
 */
#define EagleLinkedList_ForeachEnd }}

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

/**
 Create a new linked list.
 @return A new linked list.
 */
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
 * @param [in] list The list to pop from.
 * @see EagleLinkedListItem_Delete()
 * @return The linked list item on the end of the list.
 */
EagleLinkedListItem* EagleLinkedList_pop(EagleLinkedList *list);

/**
 * Check if the list is empty.
 * @param [in] list The list.
 * @return EagleTrue is there are no items on the list.
 */
EagleBoolean EagleLinkedList_isEmpty(EagleLinkedList *list);

/**
 * Delete the items (but not the list itself). This is useful for clearing out a list and reusing it.
 * @param [in] list The list.
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
 * @param [in] list The list.
 * @param [in] index The index.
 * @return The object is returned (not the linked list item). If the index is out of bounds then NULL will be returned.
 */
void* EagleLinkedList_get(EagleLinkedList *list, int index);

/**
 * Add an object to the list. This is an easier method then creating the linked list item first, but it means you have
 * to free the items with the list.
 * @param [in] list The list.
 * @param [in] obj The object.
 * @param [in] freeObj Free the object when the item is freed.
 * @param [in] free The function responsible for freeing the objec. This may be NULL if you want to use EagleMemory_Free()
 */
void EagleLinkedList_addObject(EagleLinkedList *list, void *obj, EagleBoolean freeObj, void (*free)(void *obj));

/**
 Delete an object from a linked list. This will also remove the EagleLinkedListItem that encapsulates in. If the same
 pointer exists in multiple items only the first will be removed.
 @param [in] list The list.
 @param [in] obj The object to remove.
 @return EagleTrue if the item was found and removed.
 */
EagleBoolean EagleLinkedList_deleteObject(EagleLinkedList *list, void *obj);

/**
 Get the first object on the linked list. This is not the same as EagleLinkedList_begin() which returns the
 EagleLinkedListItem rather than the object it is wrapping.
 @param [in] list The list.
 @return NULL or an object.
 */
void* EagleLinkedList_first(EagleLinkedList *list);

/**
 Get the last object on the linked list. This is not the same as EagleLinkedList_end() which returns the
 EagleLinkedListItem rather than the object it is wrapping.
 @param [in] list The list.
 @return NULL or an object.
 */
void* EagleLinkedList_last(EagleLinkedList *list);

#endif
