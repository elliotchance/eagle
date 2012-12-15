#ifndef eagle_EagleDbTable_h
#define eagle_EagleDbTable_h

#include "EagleDbColumn.h"

typedef struct {
    
    /**
     The raw name of the table.
     */
    char *name;
    
    /**
     The number of allocated \c columns.
     */
    int allocatedColumns;
    
    /**
     The number of used \c columns.
     */
    int usedColumns;
    
    /**
     The table columns.
     */
    EagleDbColumn **columns;
    
} EagleDbTable;

EagleDbTable* EagleDbTable_New(char *name);
void EagleDbTable_Delete(EagleDbTable *table);

void EagleDbTable_addColumn(EagleDbTable *table, EagleDbColumn *column);

#endif
