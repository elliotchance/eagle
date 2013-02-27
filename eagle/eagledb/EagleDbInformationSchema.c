#include <string.h>
#include <stdio.h>
#include "EagleDbInformationSchema.h"
#include "EagleDbTable.h"
#include "EagleDbTableData.h"
#include "EaglePageProviderVirtual.h"
#include "EagleMemory.h"

EagleDbInformationSchema* EagleDbInformationSchema_New(EagleDbInstance *db, const char *columnName)
{
    EagleDbInformationSchema *infoSchema = (EagleDbInformationSchema*) EagleMemory_Allocate("EagleDbInformationSchema_New.1", sizeof(EagleDbInformationSchema));
    if(NULL == infoSchema) {
        return NULL;
    }
    
    infoSchema->db = db;
    infoSchema->columnName = columnName;
    
    return infoSchema;
}

void EagleDbInformationSchema_Init(EagleDbInstance *db, EagleDbSchema *schema)
{
    int i;
    EagleDbInformationSchema *obj;
    EagleDbTableData *td;
    EagleDbTable *table;
    
    /* table definition */
    table = EagleDbTable_New("information_schema_tables");
    EagleDbTable_addColumn(table, EagleDbColumn_New("table_schema", EagleDataTypeText));
    EagleDbTable_addColumn(table, EagleDbColumn_New("table_name", EagleDataTypeText));
    
    /* replace the stream providers with virtual providers */
    td = EagleDbTableData_New(table, db->pageSize);
    for(i = 0; i < td->usedProviders; ++i) {
        EaglePageProvider *temp = td->providers[i];
        
        /* information schema object */
        obj = EagleDbInformationSchema_New(db, temp->name);
        
        td->providers[i] = (EaglePageProvider*) EaglePageProviderVirtual_New(
            db->pageSize,
            temp->name,
            temp->type,
            (void*) obj,
            (void (*)(void*)) EagleDbInformationSchema_tables_free,
            NULL,
            (int (*)(void*)) EagleDbInformationSchema_tables_pagesRemaining,
            (EaglePage* (*)(void*)) EagleDbInformationSchema_tables_nextPage,
            (void (*)(void*)) EagleDbInformationSchema_tables_reset
        );
        
        EaglePageProvider_Delete(temp);
    }
    
    EagleDbSchema_addTable(schema, td);
}

void EagleDbInformationSchema_tables_free(EagleDbInformationSchema *infoSchema)
{
    EagleDbInformationSchema_Delete(infoSchema);
}

int EagleDbInformationSchema_tables_pagesRemaining(EagleDbInformationSchema *infoSchema)
{
    int totalTables = EagleLinkedList_length(EagleDbInstance_getSchema(infoSchema->db, EagleDbSchema_DefaultSchemaName)->tables);
    return EaglePageProvider_TotalPages(totalTables - infoSchema->recordOffset, infoSchema->db->pageSize);
}

EaglePage* EagleDbInformationSchema_tables_nextPage(EagleDbInformationSchema *infoSchema)
{
    EaglePage *page;
    char **data;
    int totalTables = 0, tableCount = 0;
    
    /* count all tables */
    EagleLinkedList_Foreach(infoSchema->db->schemas, EagleDbSchema*, schema)
    {
        totalTables += EagleLinkedList_length(schema->tables);
    }
    EagleLinkedList_ForeachEnd
    
    data = (char**) EagleMemory_MultiAllocate("EagleDbInformationSchema_tables_nextPage.1", (size_t) totalTables, sizeof(char*));
    if(NULL == data) {
        return NULL;
    }
    
    EagleLinkedList_Foreach(infoSchema->db->schemas, EagleDbSchema*, schema)
    {
        EagleLinkedList_Foreach(schema->tables, EagleDbTableData*, td)
        {
            do {
                
                if(0 == strcmp("table_schema", infoSchema->columnName)) {
                    data[tableCount] = schema->name;
                    break;
                }
                
                if(0 == strcmp("table_name", infoSchema->columnName)) {
                    data[tableCount] = td->table->name;
                    break;
                }
                
            } while(0);
        
            ++tableCount;
        }
        EagleLinkedList_ForeachEnd
    }
    EagleLinkedList_ForeachEnd
    
    page = EaglePage_New(EagleDataTypeText, data, totalTables, totalTables, 0, EagleFalse);
    
    infoSchema->recordOffset += totalTables;
    return page;
}

void EagleDbInformationSchema_tables_reset(EagleDbInformationSchema *infoSchema)
{
    infoSchema->recordOffset = 0;
}

void EagleDbInformationSchema_Delete(EagleDbInformationSchema *infoSchema)
{
    if(NULL == infoSchema) {
        return;
    }
}

void EagleDbInformationSchema_Cleanup(EagleDbSchema *schema)
{
    /*int len, len2;
    EagleDbTableData *td = EagleDbSchema_getTable(schema, "information_schema_tables");
    len = EagleLinkedList_length(schema->tables);
    EagleLinkedList_deleteObject(schema->tables, td);
    len2 = EagleLinkedList_length(schema->tables);
    
    EagleDbTable_Delete(td->table);
    EagleDbTableData_Delete(td);*/
}
