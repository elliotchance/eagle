#include "DBSuite.h"
#include <stdio.h>
#include <stdlib.h>
#include "EagleDbSqlSelect.h"
#include "EagleDbSqlBinaryExpression.h"
#include "EagleDbSqlValue.h"
#include "EagleInstance.h"
#include "EagleData.h"
#include "EagleDbColumn.h"
#include "EagleDbTable.h"
#include "EagleDbTableData.h"
#include "EagleDbTuple.h"
#include "EagleDbConsole.h"
#include "EagleMemory.h"
#include "EagleDbParser.h"
#include "EagleLogger.h"
#include "EagleDbInstance.h"
#include "EagleDbSqlUnaryExpression.h"
#include "EaglePageProviderStream.h"
#include "EaglePageProviderArray.h"
#include "EagleDbInformationSchema.h"
#include "EagleDbSqlCastExpression.h"

EagleDbParser* _testSqlSelect(const char *sql)
{
    // add ;
    char *newsql = (char*) malloc(strlen(sql) + 2);
    sprintf(newsql, "%s;", sql);
    
    EagleDbParser *parser = EagleDbParser_ParseWithString(newsql);
    EagleMemory_Free(newsql);
    return parser;
}

CUNIT_TEST(DBSuite, EagleDbSqlSelect_New)
{
    EagleDbSqlSelect *select = EagleDbSqlSelect_New();
    
    CUNIT_ASSERT_NOT_NULL(select);
    if(select != NULL) {
        CUNIT_ASSERT_EQUAL_INT(select->expressionType, EagleDbSqlExpressionTypeSelect);
        
        CUNIT_ASSERT_NULL(select->tableName);
        CUNIT_ASSERT_NULL(select->whereExpression);
    }
    
    EagleDbSqlSelect_DeleteRecursive(select);
}

CUNIT_TEST(DBSuite, EagleDbSqlBinaryExpression_New)
{
    EagleDbSqlExpression *left = (EagleDbSqlExpression*) EagleDbSqlValue_NewWithInteger(123);
    EagleDbSqlExpression *right = (EagleDbSqlExpression*) EagleDbSqlValue_NewWithInteger(456);
    
    EagleDbSqlBinaryExpression *binary = EagleDbSqlBinaryExpression_New(left, EagleDbSqlBinaryExpressionOperatorPlus, right);
    CUNIT_ASSERT_NOT_NULL(binary);
    if(NULL != binary) {
        CUNIT_ASSERT_EQUAL_INT(binary->expressionType, EagleDbSqlExpressionTypeBinaryExpression);
        
        CUNIT_VERIFY_EQUAL_PTR(binary->left, left);
        CUNIT_VERIFY_EQUAL_INT(binary->op, EagleDbSqlBinaryExpressionOperatorPlus);
        CUNIT_VERIFY_EQUAL_PTR(binary->right, right);
    }
    
    EagleDbSqlBinaryExpression_DeleteRecursive(binary);
}

CUNIT_TEST(DBSuite, EagleDbSqlValue_NewWithInteger)
{
    EagleDbSqlValue *value = EagleDbSqlValue_NewWithInteger(123);
    
    CUNIT_ASSERT_NOT_NULL(value);
    if(NULL != value) {
        CUNIT_ASSERT_EQUAL_INT(value->expressionType, EagleDbSqlExpressionTypeValue);
        
        CUNIT_ASSERT_EQUAL_INT(value->type, EagleDbSqlValueTypeInteger);
        CUNIT_ASSERT_EQUAL_INT(value->value.intValue, 123);
    }
    
    EagleDbSqlValue_Delete(value);
}

EagleDbSqlExpression* _getExpression(const char *sql)
{
    EagleDbParser *p = _testSqlSelect(sql);
    if(EagleDbParser_hasError(p)) {
        CUNIT_FAIL(EagleDbParser_lastError(p), NULL);
    }
    
    EagleDbSqlSelect *select = (EagleDbSqlSelect*) p->yyparse_ast;
    CUNIT_ASSERT_NOT_NULL(EagleLinkedList_get(select->selectExpressions, 0));
    
    return EagleLinkedList_get(select->selectExpressions, 0);
}

void _testExpression(EagleDbSqlExpression *where, int usedProviders, int usedOperations, int *answers)
{
    // compile plan
    int pageSize = 10, cores = 1;
    EaglePageProviderStream *receiver = EaglePageProviderStream_New(EagleDataTypeInteger, pageSize, NULL);
    EaglePlan *plan = EaglePlan_New(pageSize, cores);
    
    // setup the table
    int *col1Data = (int*) calloc((size_t) pageSize, sizeof(int));
    for(int i = 0; i < pageSize; ++i) {
        col1Data[i] = i;
    }
    EaglePageProvider *col1 = (EaglePageProvider*) EaglePageProviderArray_New(EagleDataTypeInteger, col1Data, pageSize, pageSize, "col1");
    EaglePlan_addBufferProvider(plan, EaglePlanBufferProvider_NewWithProvider(1, col1, EagleTrue), EagleTrue);
    CUNIT_ASSERT_EQUAL_INT(EagleLinkedList_length(plan->providers), 1);
    
    EagleDbSqlExpression_CompilePlan((EagleDbSqlExpression**) where, 1, -1, plan);
    //printf("\n%s\n", EaglePlan_toString(plan));
    
    CUNIT_ASSERT_EQUAL_INT(EagleLinkedList_length(plan->providers), usedProviders);
    CUNIT_ASSERT_EQUAL_INT(EagleLinkedList_length(plan->operations), usedOperations);
    
    // execute
    EagleInstance *eagle = EagleInstance_New(cores);
    EagleInstance_addPlan(eagle, plan);
    EagleInstance_run(eagle);
    
    // validate result
    EaglePage *page = EaglePageProvider_nextPage((EaglePageProvider*) receiver);
    CUNIT_ASSERT_EQUAL_INT(receiver->totalRecords, pageSize);
    int valid = 1;
    for(int i = 0; i < pageSize; ++i) {
        //printf("\n%d != %d", page->data[i], answers[i]);
        if(((int*) page->data)[i] != answers[i]) {
            valid = 0;
            break;
        }
    }
    CUNIT_ASSERT_EQUAL_INT(valid, 1);
    
    EaglePageProvider_Delete((EaglePageProvider*) receiver);
    EagleInstance_Delete(eagle);
}

CUNIT_TEST(DBSuite, EagleDbColumn_New)
{
    EagleDbColumn *col = EagleDbColumn_New("mycol", EagleDataTypeInteger);
    CUNIT_ASSERT_EQUAL_STRING(col->name, "mycol");
    CUNIT_ASSERT_EQUAL_INT(col->type, EagleDataTypeInteger);
    EagleDbColumn_Delete(col);
}

CUNIT_TEST(DBSuite, EagleDbSqlSelect_Delete)
{
    EagleDbSqlSelect_DeleteRecursive(NULL);
}

CUNIT_TEST(DBSuite, EagleDbTable_New)
{
    EagleDbTable *table = EagleDbTable_New("mytable");
    EagleDbTable_addColumn(table, EagleDbColumn_New("col1", EagleDataTypeInteger));
    EagleDbTable_addColumn(table, EagleDbColumn_New("col2", EagleDataTypeInteger));
    
    CUNIT_ASSERT_EQUAL_STRING(table->name, "mytable");
    CUNIT_ASSERT_EQUAL_INT(EagleDbTable_countColumns(table), 2);
    CUNIT_ASSERT_EQUAL_STRING(EagleDbTable_getColumn(table, 0)->name, "col1");
    CUNIT_ASSERT_EQUAL_INT(EagleDbTable_getColumn(table, 0)->type, EagleDataTypeInteger);
    CUNIT_ASSERT_EQUAL_STRING(EagleDbTable_getColumn(table, 1)->name, "col2");
    CUNIT_ASSERT_EQUAL_INT(EagleDbTable_getColumn(table, 1)->type, EagleDataTypeInteger);
    
    EagleDbTable_DeleteWithColumns(table);
}

EagleDbTable* _getTable()
{
    /*
     CREATE TABLE mytable (
        col1 INT,
        col2 INT
     );
     */
    EagleDbTable *table = EagleDbTable_New("mytable");
    EagleDbTable_addColumn(table, EagleDbColumn_New("col1", EagleDataTypeInteger));
    EagleDbTable_addColumn(table, EagleDbColumn_New("col2", EagleDataTypeVarchar));
    EagleDbTable_addColumn(table, EagleDbColumn_New("col3", EagleDataTypeFloat));
    
    return table;
}

EagleDbTableData* _getTableWithData(int records, int recordsPerPage)
{
    // define the table
    EagleDbTable *table = _getTable();
    
    // create the data provider
    EagleDbTableData *td = EagleDbTableData_New(table, recordsPerPage);
    
    for(int i = 0; i < records; ++i) {
        // create a record
        EagleDbTuple *tuple = EagleDbTuple_New(table);
        EagleDbTuple_setInt(tuple, 0, i);
        EagleDbTuple_setInt(tuple, 1, i * 2);
        
        // put record in
        EagleDbTableData_insert(td, tuple);
    }
    
    CUNIT_ASSERT_EQUAL_INT(((EaglePageProviderStream*) td->providers[0])->totalRecords, records);
    CUNIT_ASSERT_EQUAL_INT(((EaglePageProviderStream*) td->providers[1])->totalRecords, records);
    
    return td;
}

CUNIT_TEST(DBSuite, EagleDbTuple_New)
{
    CUNIT_VERIFY_NULL(EagleDbTuple_New(NULL));
    
    EagleDbTable *table = _getTable();
    
    EagleDbTuple *tuple = EagleDbTuple_New(table);
    EagleDbTuple_setInt(tuple, 0, 123);
    EagleDbTuple_setVarchar(tuple, 1, "hello");
    EagleDbTuple_setFloat(tuple, 2, 123.456);
    
    char *desc = EagleDbTuple_toString(tuple);
    CUNIT_ASSERT_EQUAL_STRING(desc, "(col1=123,col2=\"hello\",col3=123.456)");
    EagleMemory_Free(desc);
    
    EagleDbTuple_Delete(tuple);
    EagleDbTable_DeleteWithColumns(table);
}

CUNIT_TEST(DBSuite, EagleDbSqlExpression_CompilePlan)
{
    int pageSize = 10;
    int exprs = 3;
    int totalResults = 5;
    int cores = 1;
    EagleDbSqlExpression **expr = (EagleDbSqlExpression**) calloc(exprs, sizeof(EagleDbSqlExpression*));
    
    // SELECT col1, col2 + 8 FROM mytable WHERE col1 % 2 = 1
    expr[0] = (EagleDbSqlExpression*) EagleDbSqlValue_NewWithIdentifier("col1");
    expr[1] = (EagleDbSqlExpression*) EagleDbSqlBinaryExpression_New(
        (EagleDbSqlExpression*) EagleDbSqlValue_NewWithIdentifier("col2"),
        EagleDbSqlBinaryExpressionOperatorPlus,
        (EagleDbSqlExpression*) EagleDbSqlValue_NewWithInteger(8)
    );
    expr[2] = (EagleDbSqlExpression*) EagleDbSqlBinaryExpression_New(
        (EagleDbSqlExpression*) EagleDbSqlBinaryExpression_New(
            (EagleDbSqlExpression*) EagleDbSqlValue_NewWithIdentifier("col1"),
            EagleDbSqlBinaryExpressionOperatorModulus,
            (EagleDbSqlExpression*) EagleDbSqlValue_NewWithInteger(2)
        ),
        EagleDbSqlBinaryExpressionOperatorEquals,
        (EagleDbSqlExpression*) EagleDbSqlValue_NewWithInteger(1)
    );
    
    // create the plan skeleton
    EaglePlan *plan = EaglePlan_New(pageSize, cores);
    
    // create a virtual table that consists of 2 columns; col1 and col2
    int *col1Data = (int*) calloc((size_t) pageSize, sizeof(int));
    int *col2Data = (int*) calloc((size_t) pageSize, sizeof(int));
    for(int i = 0; i < pageSize; ++i) {
        col1Data[i] = i;
        col2Data[i] = i * 2;
    }
    EaglePageProviderArray *col1 = EaglePageProviderArray_New(EagleDataTypeInteger, col1Data, pageSize, pageSize, "col1");
    EaglePageProviderArray *col2 = EaglePageProviderArray_New(EagleDataTypeInteger, col2Data, pageSize, pageSize, "col2");
    EaglePlan_addBufferProvider(plan, EaglePlanBufferProvider_NewWithProvider(1, (EaglePageProvider*) col1, EagleTrue), EagleTrue);
    EaglePlan_addBufferProvider(plan, EaglePlanBufferProvider_NewWithProvider(2, (EaglePageProvider*) col2, EagleTrue), EagleTrue);
    
    // compile plan
    EagleDbSqlExpression_CompilePlan(expr, exprs, 2, plan);
    //printf("\n%s\n", EaglePlan_toString(plan));
    
    CUNIT_ASSERT_EQUAL_INT(EagleLinkedList_length(plan->providers), 2);
    CUNIT_ASSERT_EQUAL_INT(EagleLinkedList_length(plan->operations), 5);
    
    // execute
    EagleInstance *eagle = EagleInstance_New(cores);
    EagleInstance_addPlan(eagle, plan);
    EagleInstance_run(eagle);
    
    // validate result
    CREATE_EXPRESSION_ARRAY(answers_0, pageSize, col1Data[i]);          // col1
    CREATE_EXPRESSION_ARRAY(answers_1, pageSize, col2Data[i] + 8);      // col2 + 8
    
    int **answers = (int**) calloc(exprs, sizeof(int*));
    answers[0] = (int*) calloc(totalResults, sizeof(int));
    answers[1] = (int*) calloc(totalResults, sizeof(int));
    
    for(int i = 0, j = 0; i < pageSize; ++i) {
        if(col1Data[i] % 2 == 1) {
            answers[0][j] = answers_0[i];
            answers[1][j] = answers_1[i];
            ++j;
        }
    }
    
    // subtract 1 because the WHERE clause does not get emitted
    for(int i = 0; i < plan->resultFields - 1; ++i) {
        EaglePage *page = EaglePageProvider_nextPage(plan->result[i]);
        CUNIT_ASSERT_NOT_NULL(page);
        
        // there should only be 5 records in each page because of the modulus
        CUNIT_VERIFY_EQUAL_INT(page->count, totalResults);
        
        int valid = 1;
        for(int j = 0; j < page->count; ++j) {
            if(((int*) page->data)[j] != answers[i][j]) {
                valid = 0;
                break;
            }
        }
        CUNIT_VERIFY_EQUAL_INT(valid, 1);
        
        EaglePage_Delete(page);
    }
    
    free(col1Data);
    free(col2Data);
    for(int i = 0; i < exprs; ++i) {
        EagleDbSqlExpression_DeleteRecursive(expr[i]);
    }
    for(int i = 0; i < plan->resultFields; ++i) {
        EagleMemory_Free(answers[i]);
    }
    EaglePlan_Delete(plan);
    EagleMemory_Free(answers_0);
    EagleMemory_Free(answers_1);
    EagleMemory_Free(answers);
    EagleMemory_Free(expr);
    EagleInstance_Delete(eagle);
}

CUNIT_TEST(DBSuite, EagleDbConsole_New)
{
    EagleDbConsole *console = EagleDbConsole_New();
    EagleDbConsole_Delete(console);
}

CUNIT_TEST(DBSuite, EagleDbInstance_New)
{
    EagleDbInstance *instance = EagleDbInstance_New(1000, 1);
    EagleDbInstance_Delete(instance);
}

CUNIT_TEST(DBSuite, EagleDbSqlValue_toString, 1)
{
    EagleDbSqlValue *v = EagleDbSqlValue_NewWithAsterisk();
    char *desc = EagleDbSqlValue_toString(v);
    CUNIT_VERIFY_EQUAL_STRING(desc, "*");
    free(desc);
    EagleDbSqlValue_Delete(v);
}

CUNIT_TEST(DBSuite, EagleDbSqlExpression_CompilePlanIntoBuffer, 1)
{
    EagleDbSqlSelect *select = EagleDbSqlSelect_New();
    EaglePlan *plan = EaglePlan_New(1, 1);
    
    CUNIT_VERIFY_EQUAL_INT(EagleDbSqlExpression_CompilePlanIntoBuffer_((EagleDbSqlExpression*) select, NULL, NULL, EagleTrue), EagleDbSqlExpression_ERROR);
    
    int result = EagleDbSqlExpression_CompilePlanIntoBuffer_((EagleDbSqlExpression*) select, NULL, plan, EagleTrue);
    CUNIT_VERIFY_EQUAL_INT(result, 0);
    
    EagleDbSqlSelect_DeleteRecursive(select);
    EaglePlan_Delete(plan);
}

CUNIT_TEST(DBSuite, EagleDbSqlExpression_toString)
{
    char *desc = EagleDbSqlExpression_toString(NULL);
    CUNIT_VERIFY_EQUAL_STRING(desc, "");
    free(desc);
    
    EagleDbSqlSelect *select = EagleDbSqlSelect_New();
    desc = EagleDbSqlExpression_toString((EagleDbSqlExpression*) select);
    CUNIT_VERIFY_EQUAL_STRING(desc, "SELECT  FROM ");
    free(desc);
    EagleDbSqlExpression_DeleteRecursive((EagleDbSqlExpression*) select);
}

CUNIT_TEST(DBSuite, EagleDbTuple_setInt)
{
    EagleLogger_Get()->out = NULL;
    
    EagleDbTable *table = EagleDbTable_New("mytable");
    EagleDbTable_addColumn(table, EagleDbColumn_New("a", EagleDataTypeVarchar));
    
    EagleDbTuple *tuple = EagleDbTuple_New(table);
    EagleDbTuple_setInt(tuple, 0, 123);
    CUNIT_ASSERT_LAST_ERROR("Wrong type.");
    
    EagleDbTuple_Delete(tuple);
    EagleDbTable_DeleteWithColumns(table);
}

CUNIT_TEST(DBSuite, EagleDbTuple_setVarchar)
{
    EagleLogger_Get()->out = NULL;
    
    EagleDbTable *table = EagleDbTable_New("mytable");
    EagleDbTable_addColumn(table, EagleDbColumn_New("a", EagleDataTypeInteger));
    
    EagleDbTuple *tuple = EagleDbTuple_New(table);
    EagleDbTuple_setVarchar(tuple, 0, "123");
    CUNIT_ASSERT_LAST_ERROR("Wrong type.");
    
    EagleDbTuple_Delete(tuple);
    EagleDbTable_DeleteWithColumns(table);
}

CUNIT_TEST(DBSuite, EagleDbTuple_toString)
{
    EagleDbTable *table = EagleDbTable_New("mytable");
    EagleDbTable_addColumn(table, EagleDbColumn_New("a", EagleDataTypeUnknown));
    
    EagleDbTuple *tuple = EagleDbTuple_New(table);
    char *desc = EagleDbTuple_toString(tuple);
    CUNIT_ASSERT_EQUAL_STRING(desc, "(a=?)");
    free(desc);
    
    EagleDbTuple_Delete(tuple);
    EagleDbTable_DeleteWithColumns(table);
}

CUNIT_TEST(DBSuite, EagleDbTableData_New)
{
    CUNIT_VERIFY_NULL(EagleDbTableData_New(NULL, 0));
}

CUNIT_TEST(DBSuite, EagleDbTableData_Delete)
{
    EagleDbTableData_Delete(NULL);
    
    EagleDbTable *table = EagleDbTable_New("mytable");
    EagleDbTable_addColumn(table, EagleDbColumn_New("a", EagleDataTypeInteger));
    
    EagleDbTableData *td = EagleDbTableData_New(table, 1);
    
    EagleDbTableData_Delete(td);
    EagleDbTable_DeleteWithColumns(table);
}

CUNIT_TEST(DBSuite, EagleDbTableData_insert)
{
    EagleDbTable *table = EagleDbTable_New("mytable");
    EagleDbTable_addColumn(table, EagleDbColumn_New("a", EagleDataTypeInteger));
    
    EagleDbTableData *td = EagleDbTableData_New(table, 1);
    
    EagleDbTuple *tuple = EagleDbTuple_New(table);
    EagleDbTuple_setInt(tuple, 0, 123);
    
    td->providers[0]->type = EagleDataTypeUnknown;
    EagleDbTableData_insert(td, tuple);
    EagleDbTuple_Delete(tuple);
    
    EagleDbTableData_Delete(td);
    EagleDbTable_DeleteWithColumns(table);
}

CUNIT_TEST(DBSuite, EagleDbSqlSelect_parse, 1)
{
    EagleDbSqlSelect *select = EagleDbSqlSelect_New();
    CUNIT_VERIFY_NULL(EagleDbSqlSelect_parse(NULL, NULL));
    CUNIT_VERIFY_NULL(EagleDbSqlSelect_parse(select, NULL));
    EagleDbSqlSelect_DeleteRecursive(select);
}

CUNIT_TEST(DBSuite, EagleDbSqlSelect_parse, 2)
{
    int pageSize = 1;
    char *tableName = "mytable";
    EagleDbInstance *instance = EagleDbInstance_New(pageSize, 1);
    
    EagleDbTable *table = EagleDbTable_New(tableName);
    EagleDbTable_addColumn(table, EagleDbColumn_New("a", EagleDataTypeInteger));
    
    EagleDbSqlSelect *select = EagleDbSqlSelect_New();
    select->tableName = strdup(tableName);
    select->selectExpressions = EagleLinkedList_New();
    EagleLinkedList_add(select->selectExpressions, EagleLinkedListItem_New(EagleDbSqlValue_NewWithInteger(123), EagleTrue, (void(*)(void*))EagleDbSqlValue_Delete));
    select->whereExpression = (EagleDbSqlExpression*) EagleDbSqlValue_NewWithInteger(456);
    CUNIT_ASSERT_EQUAL_INT(EagleDbSqlSelect_getFieldCount(select), 1);
    
    EagleDbSchema *schema = EagleDbInstance_getSchema(instance, EagleDbSchema_DefaultSchemaName);
    EagleDbTableData *td = EagleDbTableData_New(table, pageSize);
    EagleDbSchema_addTable(schema, td);
    
    CUNIT_ASSERT_NOT_NULL(EagleDbInstance_getTable(instance, tableName));
    
    EaglePlan *plan = EagleDbSqlSelect_parse(select, instance);
    CUNIT_VERIFY_NOT_NULL(plan);
    
    EagleDbSqlExpression_DeleteRecursive((EagleDbSqlExpression*) select);
    EagleDbTable_DeleteWithColumns(table);
    EagleDbTableData_Delete(td);
    EagleDbInstance_Delete(instance);
    EaglePlan_Delete(plan);
}

CUNIT_TEST(DBSuite, EagleDbSqlSelect_parse, 3)
{
    int pageSize = 1;
    char *tableName = "mytable";
    EagleDbInstance *instance = EagleDbInstance_New(pageSize, 1);
    
    EagleDbSqlSelect *select = EagleDbSqlSelect_New();
    select->tableName = strdup(tableName);
    select->selectExpressions = EagleLinkedList_New();
    EagleLinkedList_addObject(select->selectExpressions, EagleDbSqlValue_NewWithInteger(123), EagleTrue, (void(*)(void*)) EagleDbSqlValue_Delete);
    CUNIT_ASSERT_EQUAL_INT(EagleDbSqlSelect_getFieldCount(select), 1);
    
    CUNIT_ASSERT_NULL(EagleDbInstance_getTable(instance, tableName));
    
    EaglePlan *plan = EagleDbSqlSelect_parse(select, instance);
    CUNIT_VERIFY_NOT_NULL(plan);
    if(NULL != plan) {
        CUNIT_VERIFY_EQUAL_INT(plan->errorCode, EaglePlanErrorNoSuchTable);
        CUNIT_VERIFY_EQUAL_STRING(plan->errorMessage, tableName);
    }
    
    EagleDbSqlSelect_DeleteRecursive(select);
    EagleDbInstance_Delete(instance);
    EaglePlan_Delete(plan);
}

CUNIT_TEST(DBSuite, EagleDbInstance_PrintResults)
{
    EagleDbInstance_PrintResults(NULL);
    
    int pageSize = 1;
    EaglePlan *plan = EaglePlan_New(pageSize, 1);
    
    plan->resultFields = 1;
    plan->result = (EaglePageProvider**) calloc(plan->resultFields, sizeof(EaglePageProvider*));
    plan->result[0] = (EaglePageProvider*) EaglePageProviderStream_New(EagleDataTypeInteger, pageSize, "name");
    
    // some data
    int *data = EagleData_Int(123);
    EaglePageProvider_add(plan->result[0], data);
    free(data);
    
    EagleDbInstance_PrintResults(plan);
    EaglePageProvider_Delete(plan->result[0]);
    EaglePlan_Delete(plan);
}

CUNIT_TEST(DBSuite, EagleDbInstance_executeSelect, 1)
{
    EagleDbInstance *db = EagleDbInstance_New(1, 1);
    EagleDbSqlSelect *select = EagleDbSqlSelect_New();
    
    EagleLoggerEvent *error = NULL;
    EagleDbInstance_executeSelect(db, select, &error);
    
    EagleDbInstance_Delete(db);
    EagleDbSqlSelect_DeleteRecursive(select);
}

CUNIT_TEST(DBSuite, EagleDbInstance_executeSelect, 2)
{
    const char *tableName = "mytable";
    int pageSize = 1;
    
    EagleDbInstance *db = EagleDbInstance_New(1, 1);
    EagleDbSqlSelect *select = EagleDbSqlSelect_New();
    select->tableName = strdup(tableName);
    select->selectExpressions = EagleLinkedList_New();
    EagleLinkedList_addObject(select->selectExpressions, EagleDbSqlValue_NewWithInteger(123), EagleTrue, (void(*)(void*)) EagleDbSqlValue_Delete);
    CUNIT_ASSERT_EQUAL_INT(EagleDbSqlSelect_getFieldCount(select), 1);
    
    EagleDbTable *table = EagleDbTable_New((char*) tableName);
    EagleDbTable_addColumn(table, EagleDbColumn_New("a", EagleDataTypeInteger));
    
    EagleDbSchema *schema = EagleDbInstance_getSchema(db, EagleDbSchema_DefaultSchemaName);
    
    EagleDbTableData *td = EagleDbTableData_New(table, pageSize);
    EagleDbSchema_addTable(schema, td);
    
    EagleLoggerEvent *error = NULL;
    EagleDbInstance_executeSelect(db, select, &error);
    
    EagleDbInstance_Delete(db);
    EagleDbSqlSelect_DeleteRecursive(select);
    EagleDbTable_DeleteWithColumns(table);
    EagleDbTableData_Delete(td);
}

CUNIT_TEST(DBSuite, EagleDbInstance_executeCreateTable)
{
    EagleDbInstance *db = EagleDbInstance_New(1, 1);
    
    EagleDbTable *table = EagleDbTable_New("mytable");
    EagleDbTable_addColumn(table, EagleDbColumn_New("a", EagleDataTypeInteger));
    
    EagleLoggerEvent *error = NULL;
    CUNIT_VERIFY_TRUE(EagleDbInstance_executeCreateTable(db, table, &error));
    CUNIT_ASSERT_LAST_ERROR("Table \"default.mytable\" created.");
    
    EagleDbInstance_DeleteAll(db);
}

CUNIT_TEST(DBSuite, _DuplicateTable)
{
    EagleDbInstance *db = EagleDbInstance_New(1, 1);
    
    EagleLoggerEvent *error = NULL;
    CUNIT_VERIFY_TRUE(EagleDbInstance_execute(db, "CREATE TABLE sometable (id INT);", &error));
    CUNIT_VERIFY_EQUAL_INT(error->severity, EagleLoggerSeverityInfo);
    CUNIT_ASSERT_LAST_ERROR("Table \"default.sometable\" created.");
    
    CUNIT_VERIFY_FALSE(EagleDbInstance_execute(db, "CREATE TABLE sometable (id INT);", &error));
    CUNIT_VERIFY_EQUAL_INT(error->severity, EagleLoggerSeverityUserError);
    CUNIT_ASSERT_LAST_ERROR("Table \"default.sometable\" already exists.");
    
    EagleDbInstance_DeleteAll(db);
}

CUNIT_TEST(DBSuite, _DuplicateSchema)
{
    EagleDbInstance *db = EagleDbInstance_New(1, 1);
    EagleDbSchema *schema = EagleDbSchema_New((char*) EagleDbSchema_DefaultSchemaName);
    
    CUNIT_VERIFY_FALSE(EagleDbInstance_addSchema(db, schema));
    CUNIT_ASSERT_LAST_ERROR("Error: Schema \"default\" already exists.");
    
    EagleDbSchema_Delete(schema);
    EagleDbInstance_DeleteAll(db);
}

CUNIT_TEST(DBSuite, EagleDbInstance_execute, 1)
{
    EagleDbInstance *db = EagleDbInstance_New(1, 1);
    
    EagleLoggerEvent *error = NULL;
    CUNIT_VERIFY_TRUE(EagleDbInstance_execute(db, "CREATE TABLE sometable (id INT);", &error));
    CUNIT_ASSERT_LAST_ERROR("Table \"default.sometable\" created.");
    
    EagleDbInstance_DeleteAll(db);
}

CUNIT_TEST(DBSuite, EagleDbInstance_execute, 2)
{
    EagleDbInstance *db = EagleDbInstance_New(1, 1);
    
    EagleLoggerEvent *error = NULL;
    CUNIT_VERIFY_FALSE(EagleDbInstance_execute(db, "CREATE TABL sometable (id INT);", &error));
    CUNIT_ASSERT_LAST_ERROR("Error: syntax error, unexpected identifier, expecting TABLE");
    
    EagleDbInstance_Delete(db);
}

CUNIT_TEST(DBSuite, EagleDbInstance_execute, 3)
{
    EagleDbInstance *db = EagleDbInstance_New(1, 1);
    
    EagleLoggerEvent *error = NULL;
    CUNIT_VERIFY_FALSE(EagleDbInstance_execute(db, "SELECT * FROM mytable;", &error));
    CUNIT_ASSERT_LAST_ERROR("mytable");
    
    EagleDbInstance_Delete(db);
}

CUNIT_TEST(DBSuite, EagleDbInstance_getTable, 1)
{
    EagleDbInstance *db = EagleDbInstance_New(1, 1);
    
    EagleDbTableData *td = EagleDbInstance_getTable(db, "no_such_table");
    CUNIT_VERIFY_NULL(td);
    
    EagleDbInstance_Delete(db);
}

CUNIT_TEST(DBSuite, EagleDbInstance_getTable, 2)
{
    int pageSize = 1;
    EagleDbInstance *db = EagleDbInstance_New(pageSize, 1);
    
    EagleDbSchema *schema = EagleDbInstance_getSchema(db, EagleDbSchema_DefaultSchemaName);
    
    EagleDbTable *table1 = EagleDbTable_New("table1");
    EagleDbTableData *td1 = EagleDbTableData_New(table1, pageSize);
    EagleDbSchema_addTable(schema, td1);
    
    EagleDbTable *table2 = EagleDbTable_New("table2");
    EagleDbTableData *td2 = EagleDbTableData_New(table2, pageSize);
    EagleDbSchema_addTable(schema, td2);
    
    EagleDbTableData *td = EagleDbInstance_getTable(db, "table2");
    CUNIT_ASSERT_NOT_NULL(td);
    if(NULL != td) {
        CUNIT_VERIFY_EQUAL_STRING(td->table->name, "table2");
    }
    
    EagleDbInstance_Delete(db);
    EagleDbTableData_Delete(td1);
    EagleDbTableData_Delete(td2);
    EagleDbTable_Delete(table1);
    EagleDbTable_Delete(table2);
}

CUNIT_TEST(DBSuite, EagleDbInstance_getSchema)
{
    EagleDbInstance *db = EagleDbInstance_New(1, 1);
    
    EagleDbSchema *schema1 = EagleDbSchema_New("schema1");
    EagleDbInstance_addSchema(db, schema1);
    
    EagleDbSchema *schema2 = EagleDbSchema_New("schema2");
    EagleDbInstance_addSchema(db, schema2);
    
    EagleDbSchema *schema = EagleDbInstance_getSchema(db, "schema2");
    CUNIT_ASSERT_NOT_NULL(schema);
    if(NULL != schema) {
        CUNIT_VERIFY_EQUAL_STRING(schema->name, "schema2");
    }
    
    schema = EagleDbInstance_getSchema(db, "schema3");
    CUNIT_VERIFY_NULL(schema);
    
    EagleDbInstance_Delete(db);
    EagleDbSchema_Delete(schema1);
    EagleDbSchema_Delete(schema2);
}

CUNIT_TEST(DBSuite, EagleDbConsole_GetLine)
{
    CUNIT_ASSERT_NULL(EagleDbConsole_GetLine());
}

CUNIT_TEST(DBSuite, EagleDbConsole_run)
{
    EagleDbConsole_run(NULL);
}

CUNIT_TEST(DBSuite, EagleDbSqlSelect_Delete, 2)
{
    EagleDbSqlSelect_Delete(NULL);
}

CUNIT_TEST(DBSuite, EagleDbSqlExpression_Delete)
{
    EagleDbSqlExpression_Delete(NULL);
    
    {
        EagleDbSqlExpression *expr = (EagleDbSqlExpression*) EagleDbSqlUnaryExpression_New(EagleDbSqlUnaryExpressionOperatorNot, NULL);
        CUNIT_VERIFY_NOT_NULL(expr);
        EagleDbSqlExpression_Delete(expr);
    }
    
    {
        EagleDbSqlExpression *expr = (EagleDbSqlExpression*) EagleDbSqlBinaryExpression_New(NULL, EagleDbSqlBinaryExpressionOperatorEquals, NULL);
        CUNIT_VERIFY_NOT_NULL(expr);
        EagleDbSqlExpression_Delete(expr);
    }
    
    {
        EagleDbSqlExpression *expr = (EagleDbSqlExpression*) EagleDbSqlSelect_New();
        CUNIT_VERIFY_NOT_NULL(expr);
        EagleDbSqlExpression_Delete(expr);
    }
    
    {
        EagleDbSqlExpression *expr = (EagleDbSqlExpression*) EagleDbSqlValue_NewWithInteger(123);
        CUNIT_VERIFY_NOT_NULL(expr);
        EagleDbSqlExpression_Delete(expr);
    }
    
    {
        EagleDbSqlExpression *expr = (EagleDbSqlExpression*) EagleDbSqlCastExpression_New(NULL, EagleDataTypeInteger);
        CUNIT_VERIFY_NOT_NULL(expr);
        EagleDbSqlExpression_Delete(expr);
    }
}

CUNIT_TEST(DBSuite, EagleDbInstance_Delete)
{
    EagleDbInstance_Delete(NULL);
}

CUNIT_TEST(DBSuite, EagleDbInstance_DeleteAll)
{
    EagleDbInstance_DeleteAll(NULL);
}

CUNIT_TEST(DBSuite, EagleDbTuple_Delete)
{
    EagleDbTuple_Delete(NULL);
}

CUNIT_TEST(DBSuite, EagleDbSqlValue_Delete)
{
    EagleDbSqlValue_Delete(NULL);
}

CUNIT_TEST(DBSuite, EagleDbConsole_Delete)
{
    EagleDbConsole_Delete(NULL);
}

CUNIT_TEST(DBSuite, EagleDbTable_Delete)
{
    EagleDbTable_Delete(NULL);
}

CUNIT_TEST(DBSuite, EagleDbTable_DeleteWithColumns)
{
    EagleDbTable_DeleteWithColumns(NULL);
}

CUNIT_TEST(DBSuite, EagleDbSqlBinaryExpression_Delete)
{
    EagleDbSqlBinaryExpression_Delete(NULL);
}

CUNIT_TEST(DBSuite, EagleDbSqlBinaryExpression_DeleteRecursive)
{
    EagleDbSqlBinaryExpression_DeleteRecursive(NULL);
}

CUNIT_TEST(DBSuite, EagleDbColumn_Delete)
{
    EagleDbColumn_Delete(NULL);
}

EagleDbInstance* EagleInstanceTest(int pageSize)
{
    EagleDbInstance *db = EagleDbInstance_New(pageSize, 1);
    
    EagleDbSchema *schema =  EagleDbInstance_getSchema(db, EagleDbSchema_DefaultSchemaName);
    
    EagleDbTable *table = EagleDbTable_New("mytable");
    EagleDbTable_addColumn(table, EagleDbColumn_New("col1", EagleDataTypeInteger));
    EagleDbTable_addColumn(table, EagleDbColumn_New("col2", EagleDataTypeFloat));
    
    EagleDbTableData *td = EagleDbTableData_New(table, pageSize);
    EagleDbSchema_addTable(schema, td);
    
    return db;
}

void EagleInstanceTest_Cleanup(EagleDbInstance* db)
{
    EagleDbSchema *schema = EagleDbInstance_getSchema(db, EagleDbSchema_DefaultSchemaName);
    
    EagleLinkedList_Foreach(schema->tables, EagleDbTableData*, td)
    {
        // FIXME: This must be removed with Issue #64
        if(!strcmp(td->table->name, "information_schema_tables")) {
            continue;
        }
        
        EagleDbTable_DeleteWithColumns(td->table);
        EagleDbTableData_Delete(td);
    }
    EagleLinkedList_ForeachEnd
    
    EagleDbInstance_Delete(db);
}

CUNIT_TEST(DBSuite, _INSERT_BadTableName)
{
    EagleLogger_Get()->out = NULL;
    EagleDbInstance *db = EagleInstanceTest(10);
    
    EagleLoggerEvent *error = NULL;
    EagleBoolean success = EagleDbInstance_execute(db, "INSERT INTO mytable2 (col1) VALUES (123);", &error);
    CUNIT_ASSERT_FALSE(success);
    CUNIT_ASSERT_LAST_ERROR("No such table 'mytable2'");
    
    EagleInstanceTest_Cleanup(db);
}

CUNIT_TEST(DBSuite, _INSERT_BadColumnName)
{
    EagleDbInstance *db = EagleInstanceTest(10);
    
    EagleLoggerEvent *error = NULL;
    EagleBoolean success = EagleDbInstance_execute(db, "INSERT INTO mytable (col1000) VALUES (123);", &error);
    CUNIT_ASSERT_FALSE(success);
    CUNIT_ASSERT_LAST_ERROR("No such column 'col1000' in table 'mytable'");
    
    EagleInstanceTest_Cleanup(db);
}

CUNIT_TEST(DBSuite, _INSERT_Good)
{
    EagleDbInstance *db = EagleInstanceTest(10);
    
    // make sure there are no rows in the table
    EagleDbTableData *td = EagleDbInstance_getTable(db, "mytable");
    EaglePage *p = EaglePageProvider_nextPage(td->providers[0]);
    CUNIT_ASSERT_NULL(p);
    
    EagleLoggerEvent *error = NULL;
    EagleBoolean success = EagleDbInstance_execute(db, "INSERT INTO mytable (col1, col2) VALUES (123, 123.123);", &error);
    if(EagleFalse == success) {
        CUNIT_FAIL("%s", EagleLogger_LastEvent()->message);
    }
    
    p = EaglePageProvider_nextPage(td->providers[0]);
    CUNIT_ASSERT_NOT_NULL(p);
    CUNIT_VERIFY_EQUAL_INT(p->count, 1);
    
    EaglePage_Delete(p);
    EagleInstanceTest_Cleanup(db);
}

CUNIT_TEST(DBSuite, EagleDbTable_getColumnIndex)
{
    EagleDbTable *table = EagleDbTable_New("bla");
    EagleDbTable_addColumn(table, EagleDbColumn_New("bla", EagleDataTypeInteger));
    
    CUNIT_ASSERT_EQUAL_INT(-1, EagleDbTable_getColumnIndex(table, "nope"));
    
    EagleDbTable_DeleteWithColumns(table);
}

CUNIT_TEST(DBSuite, EagleDbSqlInsert_Delete)
{
    EagleDbSqlInsert_Delete(NULL);
}

CUnitTests* DBSuite1_tests()
{
    CUnitTests *tests = CUnitTests_New(100);
    
    // method tests
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _DuplicateSchema));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _DuplicateTable));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlInsert_Delete));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbTable_getColumnIndex));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _INSERT_Good));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _INSERT_BadColumnName));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _INSERT_BadTableName));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbColumn_Delete));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlBinaryExpression_DeleteRecursive));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlBinaryExpression_Delete));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbTable_DeleteWithColumns));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbTable_Delete));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbConsole_Delete));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlValue_Delete));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbTuple_Delete));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbInstance_Delete));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbInstance_DeleteAll));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlExpression_Delete));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlSelect_Delete, 2));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbConsole_run));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbConsole_GetLine));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbInstance_getSchema));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbInstance_getTable, 2));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbInstance_getTable, 1));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbInstance_execute, 3));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbInstance_execute, 2));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbInstance_execute, 1));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbInstance_executeCreateTable));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbInstance_executeSelect, 2));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbInstance_executeSelect, 1));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbInstance_PrintResults));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlSelect_parse, 3));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlSelect_parse, 2));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlSelect_parse, 1));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbTableData_insert));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbTableData_Delete));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbTableData_New));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbTuple_toString));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbTuple_setVarchar));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbTuple_setInt));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlExpression_toString));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlExpression_CompilePlanIntoBuffer, 1));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlValue_toString, 1));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbColumn_New));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbConsole_New));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbInstance_New));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlBinaryExpression_New));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlExpression_CompilePlan));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlSelect_New));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlSelect_Delete));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlValue_NewWithInteger));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbTable_New));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbTuple_New));
    
    return tests;
}

/**
 * The suite init function.
 */
int DBSuite_init()
{
    return 0;
}

/**
 * The suite cleanup function.
 */
int DBSuite_clean()
{
    return 0;
}
