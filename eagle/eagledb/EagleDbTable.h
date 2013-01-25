#ifndef eagle_EagleDbTable_h
#define eagle_EagleDbTable_h

#include "EagleDbColumn.h"
#include "Eagle.h"

/**
 A table definition.
 */
typedef struct {
    
    /**
     The raw name of the table.
     */
    EAGLE_ATTR_MANAGED char *name;
    
    /**
     The number of allocated \c columns.
     */
    EAGLE_ATTR_NA int allocatedColumns;
    
    /**
     The number of used \c columns.
     */
    EAGLE_ATTR_NA int usedColumns;
    
    /**
     The table columns. This is self managed because the array that contains the pointers to the columns is managed
     internally, but the columns themselves are managed externally (i.e. deleting the table will not delete the columns)
     */
    EAGLE_ATTR_SEMI_MANAGED EagleDbColumn **columns;
    
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
 * @param [in] columns Array of columns.
 * @param [in] count The amount of columns in \p columns
 */
void EagleDbTable_setColumns(EagleDbTable *table, EagleDbColumn** columns, int count);

/**
 * Free a table with its column definitions.
 * @param [in] table The table instance.
 * @see EagleDbTable_Delete()
 */
void EagleDbTable_DeleteWithColumns(EagleDbTable *table);

#endif
