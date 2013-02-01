#include <stdlib.h>
#include <string.h>
#include "EagleDbTable.h"
#include "EagleMemory.h"

EagleDbTable* EagleDbTable_New(char *name)
{
    EagleDbTable *table = (EagleDbTable*) EagleMemory_Allocate("EagleDbTable_New.1", sizeof(EagleDbTable));
    if(NULL == table) {
        return NULL;
    }
    
    table->name = (NULL == name ? NULL : strdup(name));
    table->columns = NULL;
    
    return table;
}

void EagleDbTable_setColumns(EagleDbTable *table, EagleLinkedList *columns)
{
    /* set new columns */
    table->columns = columns;
}

void EagleDbTable_Delete(EagleDbTable *table)
{
    if(NULL == table) {
        return;
    }
    
    EagleLinkedList_Delete(table->columns);
    EagleMemory_Free(table->name);
    EagleMemory_Free(table);
}

void EagleDbTable_DeleteWithColumns(EagleDbTable *table)
{
    if(NULL == table) {
        return;
    }
    
    EagleLinkedList_DeleteWithItems(table->columns);
    EagleMemory_Free(table->name);
    EagleMemory_Free(table);
}

void EagleDbTable_addColumn(EagleDbTable *table, EagleDbColumn *column)
{
    EagleLinkedListItem *item = EagleLinkedListItem_New(column, EagleTrue, (void(*)(void*)) EagleDbColumn_Delete);
    
    if(NULL == table->columns) {
        table->columns = EagleLinkedList_New();
    }
    
    EagleLinkedList_add(table->columns, item);
}

int EagleDbTable_countColumns(EagleDbTable *table)
{
    return EagleLinkedList_length(table->columns);
}

EagleDbColumn* EagleDbTable_getColumn(EagleDbTable *table, int index)
{
    return EagleLinkedList_get(table->columns, index);
}

EagleDbColumn* EagleDbTable_getColumnByName(EagleDbTable *table, char *name)
{
    EagleLinkedListItem *cursor;
    
    for(cursor = EagleLinkedList_begin(table->columns); NULL != cursor; cursor = cursor->next) {
        EagleDbColumn *col = (EagleDbColumn*) cursor->obj;
        if(0 == strcmp(col->name, name)) {
            return col;
        }
    }
    
    return NULL;
}

int EagleDbTable_getColumnIndex(EagleDbTable *table, char *name)
{
    EagleLinkedListItem *cursor;
    int i;
    
    for(cursor = EagleLinkedList_begin(table->columns), i = 0; NULL != cursor; cursor = cursor->next, ++i) {
        EagleDbColumn *col = (EagleDbColumn*) cursor->obj;
        if(0 == strcmp(col->name, name)) {
            return i;
        }
    }
    
    return -1;
}
