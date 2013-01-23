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
    table->allocatedColumns = 16;
    table->usedColumns = 0;
    table->columns = (EagleDbColumn**) EagleMemory_MultiAllocate("EagleDbTable_New.2", sizeof(EagleDbColumn*), table->allocatedColumns);
    if(NULL == table->columns) {
        EagleMemory_Free(table->name);
        EagleMemory_Free(table);
        return NULL;
    }
    
    return table;
}

void EagleDbTable_setColumns(EagleDbTable *table, EagleDbColumn** columns, int count)
{
    /* free existing memory */
    EagleMemory_Free(table->columns);
    
    /* set new columns */
    table->allocatedColumns = count;
    table->usedColumns = count;
    table->columns = columns;
}

void EagleDbTable_Delete(EagleDbTable *table)
{
    EagleMemory_Free(table->columns);
    EagleMemory_Free(table->name);
    EagleMemory_Free(table);
}

void EagleDbTable_DeleteWithColumns(EagleDbTable *table)
{
    int i;
    
    for(i = 0; i < table->usedColumns; ++i) {
        EagleDbColumn_Delete(table->columns[i]);
    }
    EagleMemory_Free(table->columns);
    
    EagleMemory_Free(table->name);
    EagleMemory_Free(table);
}

void EagleDbTable_addColumn(EagleDbTable *table, EagleDbColumn *column)
{
    table->columns[table->usedColumns++] = column;
}
