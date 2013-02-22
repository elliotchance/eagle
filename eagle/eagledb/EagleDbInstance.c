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

EagleDbInstance* EagleDbInstance_New(int pageSize)
{
    EagleDbInstance *db = (EagleDbInstance*) EagleMemory_Allocate("EagleDbInstance_New.1", sizeof(EagleDbInstance));
    EagleDbSchema *defaultSchema, *eagledbSchema;
    
    if(NULL == db) {
        return NULL;
    }
    
    db->pageSize = pageSize;
    
    /* schemas */
    db->allocatedSchemas = 10;
    db->usedSchemas = 0;
    db->schemas = (EagleDbSchema**) EagleMemory_MultiAllocate("EagleDbInstance_New.2", sizeof(EagleDbSchema*), db->allocatedSchemas);
    if(NULL == db->schemas) {
        EagleMemory_Free(db);
        return NULL;
    }
    
    defaultSchema = EagleDbSchema_New((char*) EagleDbSchema_DefaultSchemaName);
    eagledbSchema = EagleDbSchema_New((char*) EagleDbSchema_EagleSchemaName);
    EagleDbInstance_addSchema(db, defaultSchema);
    EagleDbInstance_addSchema(db, eagledbSchema);
    
    return db;
}

void EagleDbInstance_PrintResults(EaglePlan *plan)
{
    EaglePage **pages;
    int i, *widths, totalRecords = 0;
    
    if(NULL == plan) {
        return;
    }
    
    if(plan->resultFields > 0 && EaglePageProvider_pagesRemaining(plan->result[0]) > 0) {
        /* calculate the widths of the fields */
        widths = (int*) EagleMemory_MultiAllocate("EagleDbInstance_PrintResults.1", sizeof(int), plan->resultFields);
        if(NULL == widths) {
            return;
        }
        for(i = 0; i < plan->resultFields; ++i) {
            widths[i] = (int) strlen(plan->result[i]->name);
        }
    
#ifndef CUNIT
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
            int j;
            
            if(i > 0) {
                printf("+");
            }
            for(j = 0; j < widths[i] + 2; ++j) {
                printf("-");
            }
        }
        printf("\n");
#endif
    
        /* render out */
        pages = (EaglePage**) EagleMemory_MultiAllocate("EagleDbInstance_PrintResults.2", sizeof(EaglePage*), plan->resultFields);
        if(NULL == pages) {
            EagleMemory_Free(widths);
            return;
        }
        
#ifndef CUNIT
        while(1) {
            int j, k;
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
                                printf(" %*s ", widths[k], "?");
                                break;
                                
                            case EagleDataTypeInteger:
                            {
                                int *d = (int*) pages[k]->data;
                                printf(" %*d ", widths[k], d[j]);
                                break;
                            }
                                
                            case EagleDataTypeText:
                            {
                                char **d = (char**) pages[k]->data;
                                printf(" %*s ", widths[k], d[j]);
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
    
    printf("%d record%s, %.3f seconds\n\n", totalRecords, (totalRecords == 1 ? "" : "s"), EaglePlan_getExecutionSeconds(plan));
}

EagleBoolean EagleDbInstance_executeSelect(EagleDbInstance *db, EagleDbSqlSelect *select)
{
    EaglePlan *plan;
    EagleBoolean success = EagleTrue;
    
    plan = EagleDbSqlSelect_parse(select, db);
    /*printf("%s\n", EaglePlan_toString(plan));*/
    
    /* catch compilation error */
    if(EagleTrue == EaglePlan_isError(plan)) {
        EagleLogger_Log(EagleLoggerSeverityUserError, plan->errorMessage);
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

EagleBoolean EagleDbInstance_executeInsert(EagleDbInstance *db, EagleDbSqlInsert *insert)
{
    char msg[1024];
    EagleLinkedListItem *cursor;
    int i;
    EagleDbTuple *tuple;
    
    /* make the table exists */
    EagleDbTableData *td = EagleDbInstance_getTable(db, insert->table);
    if(NULL == td) {
        sprintf(msg, "No such table '%s'", insert->table);
        EagleLogger_Log(EagleLoggerSeverityUserError, msg);
        return EagleFalse;
    }
    
    /* make sure the number of columns match the number of values */
    if(EagleLinkedList_length(insert->names) != EagleLinkedList_length(insert->values)) {
        sprintf(msg, "There are %d columns and %d values", EagleLinkedList_length(insert->names),
                EagleLinkedList_length(insert->values));
        EagleLogger_Log(EagleLoggerSeverityUserError, msg);
        return EagleFalse;
    }
    
    /* make sure all the columns exist */
    for(cursor = EagleLinkedList_begin(insert->names), i = 0; NULL != cursor; cursor = cursor->next, ++i) {
        EagleDbSqlExpression *expr = (EagleDbSqlExpression*) cursor->obj, *valueExpr;
        EagleDbSqlValue *value, *valueValue;
        EagleDbColumn *col;
        
        /* they must be column names, expressions are not acceptable */
        if(EagleDbSqlExpressionTypeValue != expr->expressionType) {
            EagleLogger_Log(EagleLoggerSeverityUserError, "You cannot use expressions for column names");
            return EagleFalse;
        }
        
        value = (EagleDbSqlValue*) expr;
        if(EagleDbSqlValueTypeIdentifier != value->type) {
            EagleLogger_Log(EagleLoggerSeverityUserError, "You cannot use expressions for column names");
            return EagleFalse;
        }
        
        /* column name exists in the table? */
        col = EagleDbTable_getColumnByName(td->table, value->value.identifier);
        if(NULL == col) {
            sprintf(msg, "No such column '%s' in table '%s'", value->value.identifier, insert->table);
            EagleLogger_Log(EagleLoggerSeverityUserError, msg);
            return EagleFalse;
        }
        
        /* make sure the types are correct */
        valueExpr = (EagleDbSqlExpression*) EagleLinkedList_get(insert->values, i);
        if(EagleDbSqlExpressionTypeValue != valueExpr->expressionType) {
            sprintf(msg, "Expressions in VALUES are not yet supported for column '%s'", col->name);
            EagleLogger_Log(EagleLoggerSeverityUserError, msg);
            return EagleFalse;
        }
        
        valueValue = (EagleDbSqlValue*) valueExpr;
        if(col->type != EagleDataTypeInteger || valueValue->type != EagleDbSqlValueTypeInteger) {
            sprintf(msg, "Only integers are supported for values");
            EagleLogger_Log(EagleLoggerSeverityUserError, msg);
            return EagleFalse;
        }
    }
    
    /* everything looks good, we can create the tuple now */
    tuple = EagleDbTuple_New(td->table);
    for(cursor = EagleLinkedList_begin(insert->names), i = 0; NULL != cursor; cursor = cursor->next, ++i) {
        char *colName = ((EagleDbSqlValue*) cursor->obj)->value.identifier;
        int colIndex = EagleDbTable_getColumnIndex(td->table, colName);
        int value = ((EagleDbSqlValue*) EagleLinkedList_get(insert->values, i))->value.intValue;
        EagleDbTuple_setInt(tuple, colIndex, value);
    }
    
    /* do the INSERT */
    EagleDbTableData_insert(td, tuple);
    
    /* cleanup */
    EagleDbTuple_Delete(tuple);
    
#ifndef CUNIT
    printf("INSERT\n\n");
#endif
    
    return EagleTrue;
}

EagleBoolean EagleDbInstance_executeCreateTable(EagleDbInstance *db, EagleDbTable *table)
{
    EagleBoolean success = EagleTrue;
    char msg[1024];
    EagleDbTableData *td;
    EagleDbSchema *schema;
    
    sprintf(msg, "Table '%s' created.", table->name);
    EagleLogger_Log(EagleLoggerSeverityInfo, msg);
    
    /* create the table data */
    td = EagleDbTableData_New(table, db->pageSize);
    
    /* add table to default schema */
    schema = EagleDbInstance_getSchema(db, EagleDbSchema_DefaultSchemaName);
    EagleDbSchema_addTable(schema, td);
    
#ifndef CUNIT
    printf("%s\n\n", msg);
#endif
    
    return success;
}

EagleBoolean EagleDbInstance_execute(EagleDbInstance *db, const char *sql)
{
    EagleDbParser *p;
    EagleBoolean success = EagleTrue;
    
    /* parse sql */
    p = EagleDbParser_ParseWithString(sql);
    
    /* check for errors */
    if(EagleTrue == EagleDbParser_hasError(p)) {
        char msg[1024];
        sprintf(msg, "Error: %s", EagleDbParser_lastError(p));
        EagleLogger_Log(EagleLoggerSeverityUserError, msg);
        success = EagleFalse;
    }
    else {
        switch(p->yystatementtype) {
                
            case EagleDbSqlStatementTypeNone:
                /* lets not consider this an error and ignore it */
                break;
                
            case EagleDbSqlStatementTypeSelect:
                success = EagleDbInstance_executeSelect(db, (EagleDbSqlSelect*) p->yyparse_ast);
                EagleDbSqlSelect_DeleteRecursive((EagleDbSqlSelect*) p->yyparse_ast);
                break;
                
            case EagleDbSqlStatementTypeCreateTable:
                success = EagleDbInstance_executeCreateTable(db, (EagleDbTable*) p->yyparse_ast);
                break;
                
            case EagleDbSqlStatementTypeInsert:
                success = EagleDbInstance_executeInsert(db, (EagleDbSqlInsert*) p->yyparse_ast);
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
        int i;
        
        for(i = 0; i < db->usedSchemas; ++i) {
            if(0 == strcmp(db->schemas[i]->name, EagleDbSchema_DefaultSchemaName) ||
               0 == strcmp(db->schemas[i]->name, EagleDbSchema_EagleSchemaName)) {
                EagleDbSchema_Delete(db->schemas[i]);
            }
        }
        EagleMemory_Free(db->schemas);
        EagleMemory_Free(db);
    }
}

void EagleDbInstance_DeleteAll(EagleDbInstance *db)
{
    if(NULL == db) {
        return;
    }
    
    {
        int i, j;
        
        for(i = 0; i < db->usedSchemas; ++i) {
            for(j = 0; j < db->schemas[i]->usedTables; ++j) {
                EagleDbTable_DeleteWithColumns(db->schemas[i]->tables[j]->table);
                EagleDbTableData_Delete(db->schemas[i]->tables[j]);
            }
            EagleDbSchema_Delete(db->schemas[i]);
        }
        EagleMemory_Free(db->schemas);
        EagleMemory_Free(db);
    }
}

EagleDbTableData* EagleDbInstance_getTable(EagleDbInstance *db, char *tableName)
{
    int i;
    EagleDbSchema *schema = EagleDbInstance_getSchema(db, EagleDbSchema_DefaultSchemaName);
    
    if(NULL != schema) {
        for(i = 0; i < schema->usedTables; ++i) {
            if(0 == strcmp(tableName, schema->tables[i]->table->name)) {
                return schema->tables[i];
            }
        }
    }
    
    return NULL;
}

EagleDbSchema* EagleDbInstance_getSchema(EagleDbInstance *db, const char *schemaName)
{
    int i;
    
    for(i = 0; i < db->usedSchemas; ++i) {
        if(0 == strcmp(schemaName, db->schemas[i]->name)) {
            return db->schemas[i];
        }
    }
    
    return NULL;
}

void EagleDbInstance_addSchema(EagleDbInstance *db, EagleDbSchema *schema)
{
    db->schemas[db->usedSchemas++] = schema;
}
