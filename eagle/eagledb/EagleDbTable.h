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

/**
 Count the columns in a table definition.
 @param [in] table Table definition.
 @return The number of columns in the table.
 */
int EagleDbTable_countColumns(EagleDbTable *table);

/**
 Get a column of a table by its index. The first column will have the index of 0.
 @param [in] table Table definition.
 @param [in] index Column index.
 @return NULL if the column does not exist.
 @see EagleDbTable_getColumnIndex()
 */
EagleDbColumn* EagleDbTable_getColumn(EagleDbTable *table, int index);

/**
 Get a column of a table by its name.
 @param [in] table Table definition.
 @param [in] name Column name.
 @return NULL if the column does not exist.
 */
EagleDbColumn* EagleDbTable_getColumnByName(EagleDbTable *table, char *name);

/**
 Find the column index by its name.
 @param [in] table Table definition.
 @param [in] name Column name.
 @return -1 if the column does not exist.
 @see EagleDbTable_getColumnByName()
 */
int EagleDbTable_getColumnIndex(EagleDbTable *table, char *name);

#endif
