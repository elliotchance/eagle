#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "EagleDbSchema.h"
#include "EagleMemory.h"

EagleDbSchema* EagleDbSchema_New(char *name)
{
    EagleDbSchema *schema = (EagleDbSchema*) malloc(sizeof(EagleDbSchema));
    
    schema->name = strdup(name);
    schema->allocatedTables = 16;
    schema->usedTables = 0;
    schema->tables = (EagleDbTableData**) calloc((size_t) schema->allocatedTables, sizeof(EagleDbTableData*));
    
    return schema;
}

void EagleDbSchema_Delete(EagleDbSchema *schema)
{
    EagleMemory_Free(schema->tables);
    EagleMemory_Free(schema->name);
    EagleMemory_Free(schema);
}

void EagleDbSchema_addTable(EagleDbSchema *schema, EagleDbTableData *td)
{
    schema->tables[schema->usedTables++] = td;
}
