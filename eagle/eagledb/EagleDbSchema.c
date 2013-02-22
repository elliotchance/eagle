#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "EagleDbSchema.h"
#include "EagleMemory.h"

const char *EagleDbSchema_DefaultSchemaName = "default";

const char *EagleDbSchema_EagleSchemaName = "eagledb";

EagleDbSchema* EagleDbSchema_New(char *name)
{
    EagleDbSchema *schema = (EagleDbSchema*) EagleMemory_Allocate("EagleDbSchema_New.1", sizeof(EagleDbSchema));
    if(NULL == schema) {
        return NULL;
    }
    
    schema->name = (NULL == name ? NULL : strdup(name));
    schema->allocatedTables = 16;
    schema->usedTables = 0;
    schema->tables = (EagleDbTableData**) EagleMemory_MultiAllocate("EagleDbSchema_New.2", sizeof(EagleDbTableData*), schema->allocatedTables);
    
    return schema;
}

void EagleDbSchema_Delete(EagleDbSchema *schema)
{
    if(NULL == schema) {
        return;
    }
    
    EagleMemory_Free(schema->tables);
    EagleMemory_Free(schema->name);
    EagleMemory_Free(schema);
}

void EagleDbSchema_addTable(EagleDbSchema *schema, EagleDbTableData *td)
{
    schema->tables[schema->usedTables++] = td;
}
