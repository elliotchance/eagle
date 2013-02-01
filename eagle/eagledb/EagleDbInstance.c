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
    int i;
    EagleDbInstance *db = (EagleDbInstance*) EagleMemory_Allocate("EagleDbInstance_New.1", sizeof(EagleDbInstance));
    /*EagleDbSchema *defaultSchema, *eagledbSchema;
    EagleDbTableData *td;
    EagleDbTable *table;*/
    
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
    
    /*defaultSchema = EagleDbSchema_New("default");
    eagledbSchema = EagleDbSchema_New("eagledb");
    EagleDbInstance_addSchema(defaultSchema);
    EagleDbInstance_addSchema(eagledbSchema);*/
    
    /* create a virtual table */
    /*table = EagleDbTable_New("t");
    EagleDbTable_addColumn(table, EagleDbColumn_New("col1", EagleDataTypeInteger));
    EagleDbTable_addColumn(table, EagleDbColumn_New("col2", EagleDataTypeText));*/
    
    /* put some data in it */
    /*td = EagleDbTableData_New(table);
    EagleDbSchema_addTable(defaultSchema, td);*/
    
    for(i = 0; i < 10; ++i) {
        /* create a record */
        /*EagleDbTuple *tuple = EagleDbTuple_New(table);
        EagleDbTuple_setInt(tuple, 0, i);
        EagleDbTuple_setText(tuple, 1, "hello");*/
        
        /* put record in */
        /*EagleDbTableData_insert(db->td, tuple);*/
        /*EagleDbTuple_Delete(tuple);*/
    }
    
    return db;
}

void EagleDbInstance_PrintResults(EaglePlan *plan)
{
    int i, j, k, *widths;
    EaglePage **pages;
    int totalRecords = 0;
    
    if(NULL == plan) {
        return;
    }
    
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
    while(1) {
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
            if(NULL == pages || NULL == pages[0]) {
                break;
            }
            for(j = 0; j < pages[0]->count; ++j) {
#ifndef CUNIT
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
#endif
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
    
#ifndef CUNIT
    printf("\n%d record%s, %.3f seconds\n\n", totalRecords, (totalRecords == 1 ? "" : "s"), EaglePlan_getExecutionSeconds(plan));
#endif
    EagleMemory_Free(pages);
    EagleMemory_Free(widths);
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
    sprintf(msg, "Table '%s' created.", table->name);
    EagleLogger_Log(EagleLoggerSeverityInfo, msg);
    
#ifndef CUNIT
    printf("%s\n\n", msg);
#endif
    
    return success;
}

EagleBoolean EagleDbInstance_execute(EagleDbInstance *db, char *sql)
{
    EagleDbParser *p;
    EagleBoolean success = EagleTrue;
    
    /* parse sql */
    EagleDbParser_Init();
    EagleDbParser_LoadString(sql);
    EagleDbParser_Parse();
    
    /* check for errors */
    p = EagleDbParser_Get();
    if(EagleDbParser_HasError() > 0) {
        char msg[1024];
        sprintf(msg, "Error: %s", EagleDbParser_LastError());
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
                EagleDbTable_DeleteWithColumns((EagleDbTable*) p->yyparse_ast);
                break;
                
            case EagleDbSqlStatementTypeInsert:
                success = EagleDbInstance_executeInsert(db, (EagleDbSqlInsert*) p->yyparse_ast);
                EagleDbSqlInsert_Delete((EagleDbSqlInsert*) p->yyparse_ast);
                break;
                
        }
    }
    
    /* clean up */
    EagleDbParser_Finish();
    return success;
}

void EagleDbInstance_Delete(EagleDbInstance *db)
{
    if(NULL == db) {
        return;
    }
    
    EagleMemory_Free(db->schemas);
    EagleMemory_Free(db);
}

EagleDbTableData* EagleDbInstance_getTable(EagleDbInstance *db, char *tableName)
{
    int i;
    EagleDbSchema *schema = EagleDbInstance_getSchema(db, (char*) EagleDbSchema_DefaultSchemaName);
    if(NULL == schema) {
        return NULL;
    }
    
    for(i = 0; i < schema->usedTables; ++i) {
        if(0 == strcmp(tableName, schema->tables[i]->table->name)) {
            return schema->tables[i];
        }
    }
    
    return NULL;
}

EagleDbSchema* EagleDbInstance_getSchema(EagleDbInstance *db, char *schemaName)
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
