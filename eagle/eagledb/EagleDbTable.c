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
