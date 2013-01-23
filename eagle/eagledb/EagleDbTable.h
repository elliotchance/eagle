#ifndef eagle_EagleDbTable_h
#define eagle_EagleDbTable_h

#include "EagleDbColumn.h"
#include "Eagle.h"

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

EagleDbTable* EagleDbTable_New(char *name);

void EagleDbTable_Delete(EagleDbTable *table);

void EagleDbTable_addColumn(EagleDbTable *table, EagleDbColumn *column);

void EagleDbTable_setColumns(EagleDbTable *table, EagleDbColumn** columns, int count);

#endif
