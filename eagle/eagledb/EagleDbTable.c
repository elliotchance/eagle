#include <stdlib.h>
#include <string.h>
#include "EagleDbTable.h"

EagleDbTable* EagleDbTable_New(char *name)
{
    EagleDbTable *table = (EagleDbTable*) malloc(sizeof(EagleDbTable));
    
    table->name = strdup(name);
    table->allocatedColumns = 16;
    table->usedColumns = 0;
    table->columns = (EagleDbColumn**) calloc((size_t) table->allocatedColumns, sizeof(EagleDbColumn));
    
    return table;
}

void EagleDbTable_Delete(EagleDbTable *table)
{
    int i;
    
    for(i = 0; i < table->usedColumns; ++i) {
        EagleDbColumn_Delete(table->columns[i]);
    }
    free(table->columns);
    
    free(table->name);
    free(table);
}

void EagleDbTable_addColumn(EagleDbTable *table, EagleDbColumn *column)
{
    table->columns[table->usedColumns++] = column;
}
