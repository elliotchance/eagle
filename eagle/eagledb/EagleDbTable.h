#ifndef eagle_EagleDbTable_h
#define eagle_EagleDbTable_h

#include "EagleDbColumn.h"
#include "Eagle.h"
#include "EagleLinkedList.h"

/**
 A table definition.
 */
typedef struct {
    
    /**
     The raw name of the table.
     */
    EAGLE_ATTR_MANAGED char *name;
    
    /**
     The table columns. This is self managed because the array that contains the pointers to the columns is managed
     internally, but the columns themselves are managed externally (i.e. deleting the table will not delete the columns)
     */
    EAGLE_ATTR_SEMI_MANAGED EagleLinkedList *columns;
    
} EagleDbTable;

/**
 * Create a new table definition. You will also need to create a EagleDbTableData if you intended
 * the table to hold data.
 * @param [in] name The name of the table.
 * @return A new instance.
 */
EagleDbTable* EagleDbTable_New(char *name);

/**
 * Free a table definition. This will not free the column instances.
 * @param [in] table The instance.
 * @see EagleDbTable_DeleteWithColumns()
 */
void EagleDbTable_Delete(EagleDbTable *table);

/**
 * Add a column to the definition of a table.
 * @param [in] table The table instance.
 * @param [in] column The column definition.
 */
void EagleDbTable_addColumn(EagleDbTable *table, EagleDbColumn *column);

/**
 * Set all of the column definitions of a table.
 * @param [in] table The table instance.
 * @param [in] columns New columns.
 */
void EagleDbTable_setColumns(EagleDbTable *table, EagleLinkedList *columns);

/**
 * Free a table with its column definitions.
 * @param [in] table The table instance.
 * @see EagleDbTable_Delete()
 */
void EagleDbTable_DeleteWithColumns(EagleDbTable *table);

int EagleDbTable_countColumns(EagleDbTable *table);

EagleDbColumn* EagleDbTable_getColumn(EagleDbTable *table, int index);

EagleDbColumn* EagleDbTable_getColumnByName(EagleDbTable *table, char *name);

int EagleDbTable_getColumnIndex(EagleDbTable *table, char *name);

#endif
