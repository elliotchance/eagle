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

int _testSqlSelect(const char *sql)
{
    // add ;
    char *newsql = (char*) malloc(strlen(sql) + 2);
    sprintf(newsql, "%s;", sql);
    
    EagleDbParser_Init();
    EagleDbParser_ParseString(newsql);
    int r = yyparse();
    EagleMemory_Free(newsql);
    return r;
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
    
    EagleDbSqlExpression_Delete((EagleDbSqlExpression*) select, EagleTrue);
}

CUNIT_TEST(DBSuite, EagleDbSqlBinaryExpression_New)
{
    EagleDbSqlExpression *left = (EagleDbSqlExpression*) EagleDbSqlValue_NewWithInteger(123);
    EagleDbSqlExpression *right = (EagleDbSqlExpression*) EagleDbSqlValue_NewWithInteger(456);
    
    EagleDbSqlBinaryExpression *binary = EagleDbSqlBinaryExpression_New(left, EagleDbSqlExpressionOperatorPlus, right);
    CUNIT_ASSERT_NOT_NULL(binary);
    if(NULL != binary) {
        CUNIT_ASSERT_EQUAL_INT(binary->expressionType, EagleDbSqlExpressionTypeBinaryExpression);
        
        CUNIT_VERIFY_EQUAL_PTR(binary->left, left);
        CUNIT_VERIFY_EQUAL_INT(binary->op, EagleDbSqlExpressionOperatorPlus);
        CUNIT_VERIFY_EQUAL_PTR(binary->right, right);
    }
    
    EagleDbSqlBinaryExpression_Delete(binary, EagleTrue);
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
    if(_testSqlSelect(sql)) {
        CUNIT_FAIL(yyerrors_last(), NULL);
    }
    
    EagleDbParser *p = EagleDbParser_Get();
    EagleDbSqlSelect *select = (EagleDbSqlSelect*) p->yyparse_ast;
    CUNIT_ASSERT_NOT_NULL(select->selectExpressions[0]);
    
    return select->selectExpressions[0];
}

void _testExpression(EagleDbSqlExpression *where, int usedProviders, int usedOperations, int *answers)
{
    // compile plan
    int pageSize = 10;
    EaglePageProvider *receiver = EaglePageProvider_CreateFromStream(EagleDataTypeInteger, pageSize, NULL);
    EaglePlan *plan = EaglePlan_New(pageSize);
    
    // setup the table
    int *col1Data = (int*) calloc((size_t) pageSize, sizeof(int));
    for(int i = 0; i < pageSize; ++i) {
        col1Data[i] = i;
    }
    EaglePageProvider *col1 = EaglePageProvider_CreateFromIntArray(col1Data, pageSize, pageSize, "col1");
    EaglePlan_addBufferProvider(plan, EaglePlanBufferProvider_New(1, col1, EagleTrue), EagleTrue);
    CUNIT_ASSERT_EQUAL_INT(plan->usedProviders, 1);
    
    EagleDbSqlExpression_CompilePlan((EagleDbSqlExpression**) where, 1, -1, plan);
    //printf("\n%s\n", EaglePlan_toString(plan));
    
    CUNIT_ASSERT_EQUAL_INT(plan->usedProviders, usedProviders);
    CUNIT_ASSERT_EQUAL_INT(plan->usedOperations, usedOperations);
    
    // execute
    EagleInstance *eagle = EagleInstance_New(1);
    EagleInstance_addPlan(eagle, plan);
    EagleInstance_run(eagle);
    
    // validate result
    EaglePage *page = EaglePageProvider_nextPage(receiver);
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
    
    EaglePageProvider_Delete(receiver);
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
    EagleDbSqlSelect_Delete(NULL, EagleTrue);
}

CUNIT_TEST(DBSuite, EagleDbTable_New)
{
    EagleDbTable *table = EagleDbTable_New("mytable");
    EagleDbTable_addColumn(table, EagleDbColumn_New("col1", EagleDataTypeInteger));
    EagleDbTable_addColumn(table, EagleDbColumn_New("col2", EagleDataTypeInteger));
    
    CUNIT_ASSERT_EQUAL_STRING(table->name, "mytable");
    CUNIT_ASSERT_EQUAL_INT(table->usedColumns, 2);
    CUNIT_ASSERT_EQUAL_STRING(table->columns[0]->name, "col1");
    CUNIT_ASSERT_EQUAL_INT(table->columns[0]->type, EagleDataTypeInteger);
    CUNIT_ASSERT_EQUAL_STRING(table->columns[1]->name, "col2");
    CUNIT_ASSERT_EQUAL_INT(table->columns[1]->type, EagleDataTypeInteger);
    
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
    EagleDbTable_addColumn(table, EagleDbColumn_New("col2", EagleDataTypeText));
    
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
    
    CUNIT_ASSERT_EQUAL_INT(td->providers[0]->totalRecords, records);
    CUNIT_ASSERT_EQUAL_INT(td->providers[1]->totalRecords, records);
    
    return td;
}

CUNIT_TEST(DBSuite, EagleDbTuple_New)
{
    CUNIT_VERIFY_NULL(EagleDbTuple_New(NULL));
    
    EagleDbTable *table = _getTable();
    
    EagleDbTuple *tuple = EagleDbTuple_New(table);
    EagleDbTuple_setInt(tuple, 0, 123);
    EagleDbTuple_setText(tuple, 1, "hello");
    char *desc = EagleDbTuple_toString(tuple);
    CUNIT_ASSERT_EQUAL_STRING(desc, "(col1=123,col2=\"hello\")");
    EagleMemory_Free(desc);
    
    EagleDbTuple_Delete(tuple);
    EagleDbTable_DeleteWithColumns(table);
}

CUNIT_TEST(DBSuite, EagleDbSqlExpression_CompilePlan)
{
    int pageSize = 10;
    int exprs = 3;
    int totalResults = 5;
    EagleDbSqlExpression **expr = (EagleDbSqlExpression**) calloc(exprs, sizeof(EagleDbSqlExpression*));
    
    // SELECT col1, col2 + 8 FROM mytable WHERE col1 % 2 = 1
    expr[0] = (EagleDbSqlExpression*) EagleDbSqlValue_NewWithIdentifier("col1");
    expr[1] = (EagleDbSqlExpression*) EagleDbSqlBinaryExpression_New(
        (EagleDbSqlExpression*) EagleDbSqlValue_NewWithIdentifier("col2"),
        EagleDbSqlExpressionOperatorPlus,
        (EagleDbSqlExpression*) EagleDbSqlValue_NewWithInteger(8)
    );
    expr[2] = (EagleDbSqlExpression*) EagleDbSqlBinaryExpression_New(
        (EagleDbSqlExpression*) EagleDbSqlBinaryExpression_New(
            (EagleDbSqlExpression*) EagleDbSqlValue_NewWithIdentifier("col1"),
            EagleDbSqlExpressionOperatorModulus,
            (EagleDbSqlExpression*) EagleDbSqlValue_NewWithInteger(2)
        ),
        EagleDbSqlExpressionOperatorEquals,
        (EagleDbSqlExpression*) EagleDbSqlValue_NewWithInteger(1)
    );
    
    // create the plan skeleton
    EaglePlan *plan = EaglePlan_New(pageSize);
    
    // create a virtual table that consists of 2 columns; col1 and col2
    int *col1Data = (int*) calloc((size_t) pageSize, sizeof(int));
    int *col2Data = (int*) calloc((size_t) pageSize, sizeof(int));
    for(int i = 0; i < pageSize; ++i) {
        col1Data[i] = i;
        col2Data[i] = i * 2;
    }
    EaglePageProvider *col1 = EaglePageProvider_CreateFromIntArray(col1Data, pageSize, pageSize, "col1");
    EaglePageProvider *col2 = EaglePageProvider_CreateFromIntArray(col2Data, pageSize, pageSize, "col2");
    EaglePlan_addBufferProvider(plan, EaglePlanBufferProvider_New(1, col1, EagleTrue), EagleTrue);
    EaglePlan_addBufferProvider(plan, EaglePlanBufferProvider_New(2, col2, EagleTrue), EagleTrue);
    
    // compile plan
    EagleDbSqlExpression_CompilePlan(expr, exprs, 2, plan);
    //printf("\n%s\n", EaglePlan_toString(plan));
    
    CUNIT_ASSERT_EQUAL_INT(plan->usedProviders, 5);
    CUNIT_ASSERT_EQUAL_INT(plan->usedOperations, 5);
    
    // execute
    EagleInstance *eagle = EagleInstance_New(1);
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
    
    // suvtract 1 becuase the WHERE clause does not get emitted
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
        EagleDbSqlExpression_Delete(expr[i], EagleTrue);
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
    EagleDbInstance *instance = EagleDbInstance_New(1000);
    EagleDbInstance_Delete(instance);
}

CUNIT_TEST(DBSuite, yyerrors_push)
{
    EagleDbParser_Init();
    for(int i = 0; i < MAX_YYERRORS + 10; ++i) {
        yyerrors_push(strdup("some error"));
    }
    
    EagleDbParser *p = EagleDbParser_Get();
    CUNIT_ASSERT_EQUAL_INT(p->yyerrors_length, MAX_YYERRORS);
    EagleDbParser_Delete();
}

CUNIT_TEST(DBSuite, yyobj_push)
{
    EagleDbParser_Init();
    for(int i = 0; i < MAX_YYOBJ + 10; ++i) {
        yyobj_push(NULL);
    }
    
    EagleDbParser *p = EagleDbParser_Get();
    CUNIT_VERIFY_EQUAL_INT(p->yyobj_length, MAX_YYOBJ);
    CUNIT_VERIFY_EQUAL_INT(p->yyerrors_length, 10);
    CUNIT_VERIFY_EQUAL_STRING(yyerrors_last(), "Cannot parse SQL. Maximum depth of 256 exceeded.");
    EagleDbParser_Delete();
}

CUNIT_TEST(DBSuite, yylist_push)
{
    EagleDbParser_Init();
    yylist_new();
    for(int i = 0; i < MAX_YYLIST + 10; ++i) {
        yylist_push(NULL);
    }
    
    EagleDbParser *p = EagleDbParser_Get();
    CUNIT_VERIFY_EQUAL_INT(p->yylist_length, MAX_YYLIST);
    CUNIT_VERIFY_EQUAL_INT(p->yyerrors_length, 10);
    CUNIT_VERIFY_EQUAL_STRING(yyerrors_last(), "Cannot parse SQL. Maximum list size of 256 exceeded.");
    free(p->yylist);
    EagleDbParser_Delete();
}

CUNIT_TEST(DBSuite, yyreturn_push)
{
    EagleDbParser_Init();
    for(int i = 0; i < MAX_YYRETURN + 10; ++i) {
        yyreturn_push(NULL);
    }
    
    EagleDbParser *p = EagleDbParser_Get();
    CUNIT_VERIFY_EQUAL_INT(p->yyreturn_length, MAX_YYRETURN);
    CUNIT_VERIFY_EQUAL_INT(p->yyerrors_length, 10);
    CUNIT_VERIFY_EQUAL_STRING(yyerrors_last(), "Cannot parse SQL. Maximum return depth of 256 exceeded.");
    EagleDbParser_Delete();
}

CUNIT_TEST(DBSuite, EagleDbSqlValue_toString)
{
    EagleDbSqlValue *v = EagleDbSqlValue_NewWithAsterisk();
    char *desc = EagleDbSqlValue_toString(v);
    CUNIT_VERIFY_EQUAL_STRING(desc, "*");
    free(desc);
    EagleDbSqlValue_Delete(v);
}

CUNIT_TEST(DBSuite, EagleDbSqlExpression_CompilePlanIntoBuffer_)
{
    EagleDbSqlSelect *select = EagleDbSqlSelect_New();
    EaglePlan *plan = EaglePlan_New(1);
    
    int result = EagleDbSqlExpression_CompilePlanIntoBuffer_((EagleDbSqlExpression*) select, NULL, plan);
    CUNIT_VERIFY_EQUAL_INT(result, 0);
    
    EagleDbSqlSelect_Delete(select, EagleTrue);
    EaglePlan_Delete(plan);
}

CUNIT_TEST(DBSuite, EagleDbSqlExpression_toString)
{
    char *desc = EagleDbSqlExpression_toString(NULL);
    CUNIT_VERIFY_EQUAL_STRING(desc, "");
    free(desc);
    
    EagleDbSqlSelect *select = EagleDbSqlSelect_New();
    desc = EagleDbSqlExpression_toString((EagleDbSqlExpression*) select);
    CUNIT_VERIFY_EQUAL_STRING(desc, "SELECT");
    free(desc);
    EagleDbSqlSelect_Delete(select, EagleTrue);
}

CUNIT_TEST(DBSuite, EagleDbTuple_setInt)
{
    EagleLogger_Get()->out = NULL;
    
    EagleDbTable *table = EagleDbTable_New("mytable");
    EagleDbTable_addColumn(table, EagleDbColumn_New("a", EagleDataTypeText));
    
    EagleDbTuple *tuple = EagleDbTuple_New(table);
    EagleDbTuple_setInt(tuple, 0, 123);
    CUNIT_ASSERT_LAST_ERROR("Wrong type.");
    
    EagleDbTuple_Delete(tuple);
    EagleDbTable_DeleteWithColumns(table);
}

CUNIT_TEST(DBSuite, EagleDbTuple_setText)
{
    EagleLogger_Get()->out = NULL;
    
    EagleDbTable *table = EagleDbTable_New("mytable");
    EagleDbTable_addColumn(table, EagleDbColumn_New("a", EagleDataTypeInteger));
    
    EagleDbTuple *tuple = EagleDbTuple_New(table);
    EagleDbTuple_setText(tuple, 0, "123");
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

CUNIT_TEST(DBSuite, EagleDbSqlSelect_parse1)
{
    EagleDbSqlSelect *select = EagleDbSqlSelect_New();
    CUNIT_VERIFY_NULL(EagleDbSqlSelect_parse(NULL, NULL));
    CUNIT_VERIFY_NULL(EagleDbSqlSelect_parse(select, NULL));
    EagleDbSqlSelect_Delete(select, EagleTrue);
}

CUNIT_TEST(DBSuite, EagleDbSqlSelect_parse2)
{
    int pageSize = 1;
    char *tableName = "mytable";
    EagleDbInstance *instance = EagleDbInstance_New(pageSize);
    
    EagleDbTable *table = EagleDbTable_New(tableName);
    EagleDbTable_addColumn(table, EagleDbColumn_New("a", EagleDataTypeInteger));
    
    EagleDbSqlSelect *select = EagleDbSqlSelect_New();
    select->tableName = strdup(tableName);
    select->usedSelectExpressions = 1;
    select->selectExpressions = (EagleDbSqlExpression**) calloc(select->usedSelectExpressions, sizeof(EagleDbSqlExpression*));
    select->selectExpressions[0] = (EagleDbSqlExpression*) EagleDbSqlValue_NewWithInteger(123);
    select->whereExpression = (EagleDbSqlExpression*) EagleDbSqlValue_NewWithInteger(456);
    CUNIT_ASSERT_EQUAL_INT(EagleDbSqlSelect_getFieldCount(select), 1);
    
    EagleDbSchema *schema = EagleDbSchema_New((char*) EagleDbSchema_DefaultSchemaName);
    EagleDbInstance_addSchema(instance, schema);
    
    EagleDbTableData *td = EagleDbTableData_New(table, pageSize);
    EagleDbSchema_addTable(schema, td);
    
    CUNIT_ASSERT_NOT_NULL(EagleDbInstance_getTable(instance, tableName));
    
    EaglePlan *plan = EagleDbSqlSelect_parse(select, instance);
    CUNIT_VERIFY_NOT_NULL(plan);
    
    EagleDbSqlSelect_Delete(select, EagleTrue);
    EagleDbTable_DeleteWithColumns(table);
    EagleDbSchema_Delete(schema);
    EagleDbTableData_Delete(td);
    EagleDbInstance_Delete(instance);
    EaglePlan_Delete(plan);
}

CUNIT_TEST(DBSuite, EagleDbSqlSelect_parse3)
{
    int pageSize = 1;
    char *tableName = "mytable";
    EagleDbInstance *instance = EagleDbInstance_New(pageSize);
    
    EagleDbSqlSelect *select = EagleDbSqlSelect_New();
    select->tableName = strdup(tableName);
    select->usedSelectExpressions = 1;
    select->selectExpressions = (EagleDbSqlExpression**) calloc(select->usedSelectExpressions, sizeof(EagleDbSqlExpression*));
    select->selectExpressions[0] = (EagleDbSqlExpression*) EagleDbSqlValue_NewWithInteger(123);
    CUNIT_ASSERT_EQUAL_INT(EagleDbSqlSelect_getFieldCount(select), 1);
    
    CUNIT_ASSERT_NULL(EagleDbInstance_getTable(instance, tableName));
    
    EaglePlan *plan = EagleDbSqlSelect_parse(select, instance);
    CUNIT_VERIFY_NOT_NULL(plan);
    if(NULL != plan) {
        CUNIT_VERIFY_EQUAL_INT(plan->errorCode, EaglePlanErrorNoSuchTable);
        CUNIT_VERIFY_EQUAL_STRING(plan->errorMessage, tableName);
    }
    
    EagleDbSqlSelect_Delete(select, EagleTrue);
    EagleDbInstance_Delete(instance);
    EaglePlan_Delete(plan);
}

CUNIT_TEST(DBSuite, EagleDbInstance_PrintResults)
{
    EagleDbInstance_PrintResults(NULL);
    
    int pageSize = 1;
    EaglePlan *plan = EaglePlan_New(pageSize);
    
    plan->resultFields = 1;
    plan->result = (EaglePageProvider**) calloc(plan->resultFields, sizeof(EaglePageProvider*));
    plan->result[0] = EaglePageProvider_CreateFromStream(EagleDataTypeInteger, pageSize, "name");
    
    // some data
    int *data = EagleData_Int(123);
    EaglePageProvider_add(plan->result[0], data);
    free(data);
    
    EagleDbInstance_PrintResults(plan);
    EaglePageProvider_Delete(plan->result[0]);
    EaglePlan_Delete(plan);
}

CUNIT_TEST(DBSuite, EagleDbInstance_executeSelect1)
{
    EagleDbInstance *db = EagleDbInstance_New(1);
    EagleDbSqlSelect *select = EagleDbSqlSelect_New();
    
    EagleDbInstance_executeSelect(db, select);
    
    EagleDbInstance_Delete(db);
    EagleDbSqlSelect_Delete(select, EagleTrue);
}

CUNIT_TEST(DBSuite, EagleDbInstance_executeSelect2)
{
    const char *tableName = "mytable";
    int pageSize = 1;
    
    EagleDbInstance *db = EagleDbInstance_New(1);
    EagleDbSqlSelect *select = EagleDbSqlSelect_New();
    select->tableName = strdup(tableName);
    select->usedSelectExpressions = 1;
    select->selectExpressions = (EagleDbSqlExpression**) calloc(select->usedSelectExpressions, sizeof(EagleDbSqlExpression*));
    select->selectExpressions[0] = (EagleDbSqlExpression*) EagleDbSqlValue_NewWithInteger(123);
    CUNIT_ASSERT_EQUAL_INT(EagleDbSqlSelect_getFieldCount(select), 1);
    
    EagleDbTable *table = EagleDbTable_New((char*) tableName);
    EagleDbTable_addColumn(table, EagleDbColumn_New("a", EagleDataTypeInteger));
    
    EagleDbSchema *schema = EagleDbSchema_New((char*) EagleDbSchema_DefaultSchemaName);
    EagleDbInstance_addSchema(db, schema);
    
    EagleDbTableData *td = EagleDbTableData_New(table, pageSize);
    EagleDbSchema_addTable(schema, td);
    
    EagleDbInstance_executeSelect(db, select);
    
    EagleDbInstance_Delete(db);
    EagleDbSqlSelect_Delete(select, EagleTrue);
    EagleDbTable_DeleteWithColumns(table);
    EagleDbSchema_Delete(schema);
    EagleDbTableData_Delete(td);
}

CUNIT_TEST(DBSuite, EagleDbInstance_executeCreateTable)
{
    EagleDbInstance *db = EagleDbInstance_New(1);
    
    EagleDbTable *table = EagleDbTable_New("mytable");
    EagleDbTable_addColumn(table, EagleDbColumn_New("a", EagleDataTypeInteger));
    
    EagleDbInstance_executeCreateTable(db, table);
    CUNIT_ASSERT_LAST_ERROR("Table 'mytable' created.");
    
    EagleDbTable_DeleteWithColumns(table);
    EagleDbInstance_Delete(db);
}

CUNIT_TEST(DBSuite, EagleDbInstance_execute1)
{
    EagleDbInstance *db = EagleDbInstance_New(1);
    
    EagleDbInstance_execute(db, "CREATE TABLE sometable (id INT);");
    CUNIT_ASSERT_LAST_ERROR("Table 'sometable' created.");
    
    EagleDbInstance_Delete(db);
}

CUNIT_TEST(DBSuite, EagleDbInstance_execute2)
{
    EagleDbInstance *db = EagleDbInstance_New(1);
    
    EagleDbInstance_execute(db, "CREATE TABL sometable (id INT);");
    CUNIT_ASSERT_LAST_ERROR("Error: syntax error, unexpected IDENTIFIER, expecting K_TABLE");
    
    EagleDbInstance_Delete(db);
}

CUNIT_TEST(DBSuite, EagleDbInstance_execute3)
{
    EagleDbInstance *db = EagleDbInstance_New(1);
    
    EagleDbInstance_execute(db, "SELECT * FROM mytable;");
    CUNIT_ASSERT_LAST_ERROR("mytable");
    
    EagleDbInstance_Delete(db);
}

CUNIT_TEST(DBSuite, EagleDbInstance_getTable1)
{
    EagleDbInstance *db = EagleDbInstance_New(1);
    
    EagleDbSchema *schema = EagleDbSchema_New((char*) EagleDbSchema_DefaultSchemaName);
    EagleDbInstance_addSchema(db, schema);
    
    EagleDbTableData *td = EagleDbInstance_getTable(db, "so_such_table");
    CUNIT_VERIFY_NULL(td);
    
    EagleDbInstance_Delete(db);
    EagleDbSchema_Delete(schema);
}

CUNIT_TEST(DBSuite, EagleDbInstance_getTable2)
{
    int pageSize = 1;
    EagleDbInstance *db = EagleDbInstance_New(pageSize);
    
    EagleDbSchema *schema = EagleDbSchema_New((char*) EagleDbSchema_DefaultSchemaName);
    EagleDbInstance_addSchema(db, schema);
    
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
    EagleDbSchema_Delete(schema);
    EagleDbTableData_Delete(td1);
    EagleDbTableData_Delete(td2);
    EagleDbTable_Delete(table1);
    EagleDbTable_Delete(table2);
}

CUNIT_TEST(DBSuite, EagleDbInstance_getSchema)
{
    EagleDbInstance *db = EagleDbInstance_New(1);
    
    EagleDbSchema *schema1 = EagleDbSchema_New("schema1");
    EagleDbInstance_addSchema(db, schema1);
    
    EagleDbSchema *schema2 = EagleDbSchema_New("schema2");
    EagleDbInstance_addSchema(db, schema2);
    
    EagleDbSchema *schema = EagleDbInstance_getSchema(db, "schema2");
    CUNIT_ASSERT_NOT_NULL(schema);
    if(NULL != schema) {
        CUNIT_VERIFY_EQUAL_STRING(schema->name, "schema2");
    }
    
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

CUnitTests* DBSuite_tests()
{
    CUnitTests *tests = CUnitTests_New(100);
    
    // method tests
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbConsole_run));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbConsole_GetLine));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbInstance_getSchema));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbInstance_getTable2));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbInstance_getTable1));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbInstance_execute3));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbInstance_execute2));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbInstance_execute1));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbInstance_executeCreateTable));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbInstance_executeSelect2));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbInstance_executeSelect1));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbInstance_PrintResults));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlSelect_parse3));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlSelect_parse2));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlSelect_parse1));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbTableData_insert));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbTableData_Delete));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbTableData_New));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbTuple_toString));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbTuple_setText));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbTuple_setInt));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlExpression_toString));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlExpression_CompilePlanIntoBuffer_));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlValue_toString));
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
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, yyerrors_push));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, yyobj_push));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, yylist_push));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, yyreturn_push));
    
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
