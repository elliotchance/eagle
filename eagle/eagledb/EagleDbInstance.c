#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "EagleDbInstance.h"
#include "EagleDbTable.h"
#include "EagleDbTuple.h"
#include "EagleDbSqlSelect.h"
#include "EaglePageProvider.h"
#include "EagleInstance.h"
#include "EagleDbTableData.h"
#include "EagleMemory.h"
#include "EagleDbInstance.h"
#include "EagleDbParser.h"
#include "EagleLogger.h"
#include "EagleDbSchema.h"
#include "EagleDbInformationSchema.h"
#include "EagleDataType.h"

EagleDbInstance* EagleDbInstance_New(int pageSize)
{
    EagleDbInstance *db = (EagleDbInstance*) EagleMemory_Allocate("EagleDbInstance_New.1", sizeof(EagleDbInstance));
    EagleDbSchema *defaultSchema, *infoSchema;
    
    if(NULL == db) {
        return NULL;
    }
    
    db->pageSize = pageSize;
    
    /* schemas */
    db->schemas = EagleLinkedList_New();
    
    defaultSchema = EagleDbSchema_New((char*) EagleDbSchema_DefaultSchemaName);
    infoSchema = EagleDbSchema_New((char*) EagleDbSchema_InformationSchemaName);
    EagleDbInstance_addSchema(db, defaultSchema);
    EagleDbInstance_addSchema(db, infoSchema);
    
    /* setup information schema */
    EagleDbInformationSchema_Init(db, defaultSchema);
    
    return db;
}

void EagleDbInstance_PrintResults(EaglePlan *plan)
{
    EaglePage **pages;
    int i, totalRecords = 0, j;
    unsigned long *widths;
    
    if(NULL == plan) {
        return;
    }
    
    if(plan->resultFields > 0 && EaglePageProvider_pagesRemaining(plan->result[0]) > 0) {
        /* calculate the widths of the fields */
        widths = (unsigned long*) EagleMemory_MultiAllocate("EagleDbInstance_PrintResults.1", sizeof(unsigned long), plan->resultFields);
        if(NULL == widths) {
            return;
        }
        for(i = 0; i < plan->resultFields; ++i) {
            widths[i] = strlen(plan->result[i]->name);
        }
        
#ifndef CUNIT
        for(i = 0; i < plan->resultFields; ++i) {
            EaglePage *page;
            
            while(NULL != (page = EaglePageProvider_nextPage(plan->result[i]))) {
                for(j = 0; j < page->count; ++j) {
                    switch(page->type) {
                            
                        case EagleDataTypeInteger:
                        {
                            char buf[30];
                            unsigned long len;
                            sprintf(buf, "%d", ((EagleDataTypeIntegerType*) page->data)[j]);
                            len = strlen(buf);
                            
                            if(len > widths[i]) {
                                widths[i] = len;
                            }
                            
                            break;
                        }
                            
                        case EagleDataTypeVarchar:
                        {
                            unsigned long len = strlen(((EagleDataTypeVarcharType*) page->data)[j]);
                            
                            if(len > widths[i]) {
                                widths[i] = len;
                            }
                            
                            break;
                        }
                            
                        case EagleDataTypeFloat:
                        {
                            char buf[30];
                            unsigned long len;
                            sprintf(buf, "%g", ((EagleDataTypeFloatType*) page->data)[j]);
                            len = strlen(buf);
                            
                            if(len > widths[i]) {
                                widths[i] = len;
                            }
                            
                            break;
                        }
                            
                        case EagleDataTypeUnknown:
                            break;
                            
                    }
                }
            }
            
            EaglePage_Delete(page);
        }
        for(i = 0; i < plan->resultFields; ++i) {
            EaglePageProvider_reset(plan->result[i]);
        }
        
        /* heading */
        printf("\n");
        for(i = 0; i < plan->resultFields; ++i) {
            if(i > 0) {
                printf("|");
            }
            printf(" %s ", plan->result[i]->name);
        }
        printf("\n");
        
        for(i = 0; i < plan->resultFields; ++i) {
            if(i > 0) {
                printf("+");
            }
            for(j = 0; j < (int) widths[i] + 2; ++j) {
                printf("-");
            }
        }
        printf("\n");
#else
        /* this is just so the compile doesn't give a warning that totalRecords is not used when CUNIT is running */
        j = 0;
#endif
    
        /* render out */
        pages = (EaglePage**) EagleMemory_MultiAllocate("EagleDbInstance_PrintResults.2", sizeof(EaglePage*), plan->resultFields);
        if(NULL == pages) {
            EagleMemory_Free(widths);
            return;
        }
        
#ifndef CUNIT
        while(1) {
            int k;
            int finished = 0;
            
            for(i = 0; i < plan->resultFields; ++i) {
                EaglePage *page = EaglePageProvider_nextPage(plan->result[i]);
                if(NULL == page) {
                    finished = 1;
                    break;
                }
                pages[i] = page;
            }
            
            if(finished == 0) {
                for(j = 0; j < pages[0]->count; ++j) {
                    for(k = 0; k < plan->resultFields; ++k) {
                        if(k > 0) {
                            printf("|");
                        }
                        
                        switch(pages[k]->type) {
                                
                            case EagleDataTypeUnknown:
                                printf(" %*s ", (int) widths[k], "?");
                                break;
                                
                            case EagleDataTypeInteger:
                            {
                                EagleDataTypeIntegerType *d = (EagleDataTypeIntegerType*) pages[k]->data;
                                printf(" %*d ", (int) widths[k], d[j]);
                                break;
                            }
                                
                            case EagleDataTypeVarchar:
                            {
                                EagleDataTypeVarcharType *d = (EagleDataTypeVarcharType*) pages[k]->data;
                                printf(" %-*s ", (int) widths[k], d[j]);
                                break;
                            }
                                
                            case EagleDataTypeFloat:
                            {
                                EagleDataTypeFloatType *d = (EagleDataTypeFloatType*) pages[k]->data;
                                printf(" %*g ", (int) widths[k], d[j]);
                                break;
                            }
                                
                        }
                    }
                    printf("\n");
                    ++totalRecords;
                }
                
                for(k = 0; k < plan->resultFields; ++k) {
                    EaglePage_Delete(pages[k]);
                }
            }
            else {
                break;
            }
        }
        
        printf("\n");
#endif
        EagleMemory_Free(widths);
        EagleMemory_Free(pages);
    }
    
#ifndef CUNIT
    printf("%d record%s, %.3f seconds\n\n", totalRecords, (totalRecords == 1 ? "" : "s"), EaglePlan_getExecutionSeconds(plan));
#else
    /* this is just so the compile doesn't give a warning that totalRecords is not used when CUNIT is running */
    totalRecords = 0;
#endif
}

EagleBoolean EagleDbInstance_executeSelect(EagleDbInstance *db, EagleDbSqlSelect *select, EagleLoggerEvent **error)
{
    EaglePlan *plan;
    EagleBoolean success = EagleTrue;
    
    plan = EagleDbSqlSelect_parse(select, db);
    /*printf("%s\n", EaglePlan_toString(plan));*/
    
    /* catch compilation error */
    if(EagleTrue == EaglePlan_isError(plan)) {
        *error = EagleLogger_Log(EagleLoggerSeverityUserError, plan->errorMessage);
        success = EagleFalse;
    }
    else {
        /* execute */
        EagleInstance *eagle = EagleInstance_New(1);
        EagleInstance_addPlan(eagle, plan);
        EagleInstance_run(eagle);
        
        /* print results */
        EagleDbInstance_PrintResults(plan);
        EagleInstance_Delete(eagle);
    }
    
    EaglePlan_Delete(plan);
    return success;
}

EagleBoolean EagleDbInstance_executeInsert(EagleDbInstance *db, EagleDbSqlInsert *insert, EagleLoggerEvent **error)
{
    char msg[1024];
    EagleLinkedListItem *cursor;
    int i, rowsInserted = 1;
    EagleDbTuple *tuple;
    
    /* make the table exists */
    EagleDbTableData *td = EagleDbInstance_getTable(db, insert->table);
    if(NULL == td) {
        sprintf(msg, "No such table '%s'", insert->table);
        *error = EagleLogger_Log(EagleLoggerSeverityUserError, msg);
        return EagleFalse;
    }
    
    /* make sure the number of columns match the number of values */
    if(EagleLinkedList_length(insert->names) != EagleLinkedList_length(insert->values)) {
        sprintf(msg, "There are %d columns and %d values", EagleLinkedList_length(insert->names),
                EagleLinkedList_length(insert->values));
        *error = EagleLogger_Log(EagleLoggerSeverityUserError, msg);
        return EagleFalse;
    }
    
    /* make sure all the columns exist */
    for(cursor = EagleLinkedList_begin(insert->names), i = 0; NULL != cursor; cursor = cursor->next, ++i) {
        EagleDbSqlExpression *expr = (EagleDbSqlExpression*) cursor->obj, *valueExpr;
        EagleDbSqlValue *value;
        EagleDbColumn *col;
        
        /* they must be column names, expressions are not acceptable */
        if(EagleDbSqlExpressionTypeValue != expr->expressionType) {
            *error = EagleLogger_Log(EagleLoggerSeverityUserError, "You cannot use expressions for column names");
            return EagleFalse;
        }
        
        value = (EagleDbSqlValue*) expr;
        if(EagleDbSqlValueTypeIdentifier != value->type) {
            *error = EagleLogger_Log(EagleLoggerSeverityUserError, "You cannot use expressions for column names");
            return EagleFalse;
        }
        
        /* column name exists in the table? */
        col = EagleDbTable_getColumnByName(td->table, value->value.identifier);
        if(NULL == col) {
            sprintf(msg, "No such column '%s' in table '%s'", value->value.identifier, insert->table);
            *error = EagleLogger_Log(EagleLoggerSeverityUserError, msg);
            return EagleFalse;
        }
        
        /* make sure the types are correct */
        valueExpr = (EagleDbSqlExpression*) EagleLinkedList_get(insert->values, i);
        if(EagleDbSqlExpressionTypeValue != valueExpr->expressionType) {
            sprintf(msg, "Expressions in VALUES are not yet supported for column '%s'", col->name);
            *error = EagleLogger_Log(EagleLoggerSeverityUserError, msg);
            return EagleFalse;
        }
    }
    
    /* everything looks good, we can create the tuple now */
    tuple = EagleDbTuple_New(td->table);
    
    for(cursor = EagleLinkedList_begin(insert->names), i = 0; NULL != cursor; cursor = cursor->next, ++i) {
        char *colName = ((EagleDbSqlValue*) cursor->obj)->value.identifier;
        int colIndex = EagleDbTable_getColumnIndex(td->table, colName);
        
        EagleDbSqlValue *v = ((EagleDbSqlValue*) EagleLinkedList_get(insert->values, i));
        switch(v->type) {
                
            case EagleDbSqlValueTypeInteger:
            {
                int value = v->value.intValue;
                EagleDbTuple_setInt(tuple, colIndex, value);
                break;
            }
                
            case EagleDbSqlValueTypeString:
            {
                char *value = v->value.identifier;
                EagleDbTuple_setVarchar(tuple, colIndex, value);
                break;
            }
                
            case EagleDbSqlValueTypeFloat:
            {
                int value = v->value.intValue;
                EagleDbTuple_setInt(tuple, colIndex, value);
                break;
            }
                
            case EagleDbSqlValueTypeAsterisk:
            case EagleDbSqlValueTypeIdentifier:
            {
                EagleDbTuple_Delete(tuple);
                
                sprintf(msg, "Only literal values are allowed for expressions.");
                *error = EagleLogger_Log(EagleLoggerSeverityUserError, msg);
                return EagleFalse;
            }
            
        }
        
    }
    
    /* do the INSERT */
    EagleDbTableData_insert(td, tuple);
    
    /* cleanup */
    EagleDbTuple_Delete(tuple);
    
#ifndef CUNIT
    printf("%d row%s inserted\n\n", rowsInserted, (rowsInserted == 1 ? "" : "s"));
#else
    /* this is so the compiler does not complain about unused variable while in CUNIT */
    rowsInserted = 0;
#endif
    
    return EagleTrue;
}

EagleBoolean EagleDbInstance_executeCreateTable(EagleDbInstance *db, EagleDbTable *table, EagleLoggerEvent **error)
{
    EagleBoolean success = EagleTrue;
    char msg[1024];
    EagleDbTableData *td;
    EagleDbSchema *schema;
    
    /* create the table data */
    td = EagleDbTableData_New(table, db->pageSize);
    
    /* add table to default schema */
    schema = EagleDbInstance_getSchema(db, EagleDbSchema_DefaultSchemaName);
    if(EagleTrue == EagleDbSchema_addTable(schema, td)) {
        sprintf(msg, "Table \"%s.%s\" created.", schema->name, table->name);
        *error = EagleLogger_Log(EagleLoggerSeverityInfo, msg);
    }
    else {
        success = EagleFalse;
        sprintf(msg, "Table \"%s.%s\" already exists.", schema->name, table->name);
        *error = EagleLogger_Log(EagleLoggerSeverityUserError, msg);
        
        /* clean up resources */
        EagleDbTable_DeleteWithColumns(table);
        EagleDbTableData_Delete(td);
    }
    
#ifndef CUNIT
    printf("%s\n\n", msg);
#endif
    
    return success;
}

EagleBoolean EagleDbInstance_execute(EagleDbInstance *db, const char *sql, EagleLoggerEvent **error)
{
    EagleDbParser *p;
    EagleBoolean success = EagleTrue;
    
    /* parse sql */
    p = EagleDbParser_ParseWithString(sql);
    
    /* check for errors */
    if(EagleTrue == EagleDbParser_hasError(p)) {
        char msg[1024];
        sprintf(msg, "Error: %s", EagleDbParser_lastError(p));
        *error = EagleLogger_Log(EagleLoggerSeverityUserError, msg);
        success = EagleFalse;
    }
    else {
        switch(p->yystatementtype) {
                
            case EagleDbSqlStatementTypeNone:
                /* lets not consider this an error and ignore it */
                break;
                
            case EagleDbSqlStatementTypeSelect:
                success = EagleDbInstance_executeSelect(db, (EagleDbSqlSelect*) p->yyparse_ast, error);
                EagleDbSqlSelect_DeleteRecursive((EagleDbSqlSelect*) p->yyparse_ast);
                break;
                
            case EagleDbSqlStatementTypeCreateTable:
                success = EagleDbInstance_executeCreateTable(db, (EagleDbTable*) p->yyparse_ast, error);
                break;
                
            case EagleDbSqlStatementTypeInsert:
                success = EagleDbInstance_executeInsert(db, (EagleDbSqlInsert*) p->yyparse_ast, error);
                EagleDbSqlInsert_Delete((EagleDbSqlInsert*) p->yyparse_ast);
                break;
                
        }
    }
    
    /* clean up */
    EagleDbParser_Delete(p);
    
    return success;
}

void EagleDbInstance_Delete(EagleDbInstance *db)
{
    if(NULL == db) {
        return;
    }
    
    {
        /* always clean up the information schema */
        EagleDbInformationSchema_Cleanup(db);
        
        EagleLinkedList_Foreach(db->schemas, EagleDbSchema*, schema)
        {
            if(0 == strcmp(schema->name, EagleDbSchema_DefaultSchemaName) ||
               0 == strcmp(schema->name, EagleDbSchema_InformationSchemaName)) {
                EagleDbSchema_Delete(schema);
            }
        }
        EagleLinkedList_ForeachEnd
        
        EagleLinkedList_DeleteWithItems(db->schemas);
        EagleMemory_Free(db);
        
        /* clean up logger */
        EagleLogger_reset(EagleLogger_Get());
    }
}

void EagleDbInstance_DeleteAll(EagleDbInstance *db)
{
    if(NULL == db) {
        return;
    }
    
    {
        /* always clean up the information schema */
        EagleDbInformationSchema_Cleanup(db);
        
        EagleLinkedList_Foreach(db->schemas, EagleDbSchema*, schema)
        {
            EagleLinkedList_Foreach(schema->tables, EagleDbTableData*, table)
            {
                EagleDbTable_DeleteWithColumns(table->table);
                EagleDbTableData_Delete(table);
            }
            EagleLinkedList_ForeachEnd
            
            EagleDbSchema_Delete(schema);
        }
        EagleLinkedList_ForeachEnd
        
        EagleLinkedList_DeleteWithItems(db->schemas);
        EagleMemory_Free(db);
        
        /* clean up logger */
        EagleLogger_reset(EagleLogger_Get());
    }
}

EagleDbTableData* EagleDbInstance_getTable(EagleDbInstance *db, char *tableName)
{
    EagleDbSchema *schema;
    
    if(NULL == tableName) {
        return NULL;
    }
    
    schema = EagleDbInstance_getSchema(db, EagleDbSchema_DefaultSchemaName);
    if(NULL != schema) {
        EagleLinkedList_Foreach(schema->tables, EagleDbTableData*, table)
        {
            if(0 == strcmp(tableName, table->table->name)) {
                return table;
            }
        }
        EagleLinkedList_ForeachEnd
    }
    
    return NULL;
}

EagleDbSchema* EagleDbInstance_getSchema(EagleDbInstance *db, const char *schemaName)
{
    EagleLinkedList_Foreach(db->schemas, EagleDbSchema*, schema)
    {
        if(0 == strcmp(schemaName, schema->name)) {
            return schema;
        }
    }
    EagleLinkedList_ForeachEnd
    
    return NULL;
}

EagleBoolean EagleDbInstance_addSchema(EagleDbInstance *db, EagleDbSchema *schema)
{
    /* check if the schema exists */
    if(NULL != EagleDbInstance_getSchema(db, schema->name)) {
        char msg[1024];
        sprintf(msg, "Error: Schema \"%s\" already exists.", schema->name);
        EagleLogger_Log(EagleLoggerSeverityUserError, msg);
        return EagleFalse;
    }
    
    EagleLinkedList_addObject(db->schemas, schema, EagleFalse, NULL);
    
    return EagleTrue;
}
