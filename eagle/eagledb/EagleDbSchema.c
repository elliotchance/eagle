#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "EagleDbSchema.h"
#include "EagleMemory.h"
#include "EagleLogger.h"

const char *EagleDbSchema_DefaultSchemaName = "default";

const char *EagleDbSchema_InformationSchemaName = "information_schema";

EagleDbSchema* EagleDbSchema_New(char *name)
{
    EagleDbSchema *schema = (EagleDbSchema*) EagleMemory_Allocate("EagleDbSchema_New.1", sizeof(EagleDbSchema));
    if(NULL == schema) {
        return NULL;
    }
    
    schema->name = (NULL == name ? NULL : strdup(name));
    schema->tables = EagleLinkedList_New();
    
    return schema;
}

void EagleDbSchema_Delete(EagleDbSchema *schema)
{
    if(NULL == schema) {
        return;
    }
    
    EagleLinkedList_DeleteWithItems(schema->tables);
    EagleMemory_Free(schema->name);
    EagleMemory_Free(schema);
}

EagleDbTableData* EagleDbSchema_getTable(EagleDbSchema *schema, const char *tableName)
{
    EagleLinkedList_Foreach(schema->tables, EagleDbTableData*, table)
    {
        if(0 == strcmp(tableName, table->table->name)) {
            return table;
        }
    }
    EagleLinkedList_ForeachEnd
    
    return NULL;
}

EagleBoolean EagleDbSchema_addTable(EagleDbSchema *schema, EagleDbTableData *td)
{
    /* check if the table already exists */
    if(NULL != EagleDbSchema_getTable(schema, td->table->name)) {
        char msg[1024];
        sprintf(msg, "Error: Table \"%s.%s\" already exists.", schema->name, td->table->name);
        EagleLogger_Log(EagleLoggerSeverityUserError, msg);
        return EagleFalse;
    }
    
    EagleLinkedList_addObject(schema->tables, td, EagleFalse, NULL);
    
    return EagleTrue;
}
