#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "EagleDbSchema.h"

EagleDbSchema* EagleDbSchema_New(char *name)
{
    EagleDbSchema *schema = (EagleDbSchema*) malloc(sizeof(EagleDbSchema));
    
    schema->name = strdup(name);
    schema->allocatedTables = 16;
    schema->usedTables = 0;
    schema->tables = (EagleDbTable**) calloc((size_t) schema->allocatedTables, sizeof(EagleDbTable*));
    
    return schema;
}

void EagleDbSchema_Delete(EagleDbSchema *schema)
{
    int i;
    
    for(i = 0; i < schema->usedTables; ++i) {
        EagleDbTable_Delete(schema->tables[i]);
    }
    free(schema->tables);
    
    free(schema->name);
    free(schema);
}
