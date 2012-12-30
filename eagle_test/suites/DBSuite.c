#include <stdio.h>
#include <stdlib.h>
#include "DBSuite.h"
#include "EagleDbSqlSelect.h"
#include "EagleDbSqlBinaryExpression.h"
#include "EagleDbSqlValue.h"
#include "EagleInstance.h"
#include "EagleData.h"
#include "EagleDbColumn.h"
#include "EagleDbTable.h"
#include "EagleDbTableData.h"
#include "EagleDbTuple.h"

extern void *yyparse_ast;
extern int yyerrors_length;
char* yyerrors_last();
void yylex_init();
void yylex_free();

int _testSqlSelect(const char *sql)
{
    // add ;
    char *newsql = (char*) malloc(strlen(sql) + 2);
    sprintf(newsql, "%s;", sql);
    
    yylex_init();
    yy_scan_string(newsql);
    int r = yyparse();
    free(newsql);
    return r;
}

CUNIT_TEST(DBSuite, _, BLANK)
{
    if(_testSqlSelect("")) {
        CUNIT_FAIL(yyerrors_last(), NULL);
    }
    yylex_free();
}

CUNIT_TEST(DBSuite, EagleDbSqlSelect_New)
{
    EagleDbSqlSelect *select = EagleDbSqlSelect_New();
    
    CUNIT_ASSERT_NOT_NULL(select);
    CUNIT_ASSERT_EQUAL_INT(select->expressionType, EagleDbSqlExpressionTypeSelect);
    
    CUNIT_ASSERT_NULL(select->tableName);
    CUNIT_ASSERT_NULL(select->whereExpression);
    
    EagleDbSqlExpression_Delete((EagleDbSqlExpression*) select);
}

CUNIT_TEST(DBSuite, EagleDbSqlBinaryExpression_New)
{
    EagleDbSqlExpression *left = (EagleDbSqlExpression*) EagleDbSqlValue_NewWithInteger(123);
    EagleDbSqlExpression *right = (EagleDbSqlExpression*) EagleDbSqlValue_NewWithInteger(456);
    
    EagleDbSqlBinaryExpression *binary = EagleDbSqlBinaryExpression_New(left, EagleDbSqlExpressionOperatorPlus, right);
    CUNIT_ASSERT_NOT_NULL(binary);
    CUNIT_ASSERT_EQUAL_INT(binary->expressionType, EagleDbSqlExpressionTypeBinaryExpression);
    
    CUNIT_VERIFY_EQUAL_PTR(binary->left, left);
    CUNIT_VERIFY_EQUAL_INT(binary->op, EagleDbSqlExpressionOperatorPlus);
    CUNIT_VERIFY_EQUAL_PTR(binary->right, right);
    
    EagleDbSqlBinaryExpression_Delete(binary);
}

CUNIT_TEST(DBSuite, EagleDbSqlValue_NewWithInteger)
{
    EagleDbSqlValue *value = EagleDbSqlValue_NewWithInteger(123);
    
    CUNIT_ASSERT_NOT_NULL(value);
    CUNIT_ASSERT_EQUAL_INT(value->expressionType, EagleDbSqlExpressionTypeValue);
    
    CUNIT_ASSERT_EQUAL_INT(value->type, EagleDbSqlValueTypeInteger);
    CUNIT_ASSERT_EQUAL_INT(value->value.intValue, 123);
    
    EagleDbSqlValue_Delete(value);
}

EagleDbSqlExpression* _getExpression(const char *sql)
{
    if(_testSqlSelect(sql)) {
        CUNIT_FAIL(yyerrors_last(), NULL);
    }
    
    EagleDbSqlSelect *select = (EagleDbSqlSelect*) yyparse_ast;
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
    EaglePlan_addBufferProvider(plan, EaglePlanBufferProvider_New(1, col1, EagleTrue));
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
    EagleDbSqlSelect_Delete(NULL);
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
    
    EagleDbTable_Delete(table);
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
    EagleDbTableData *td = EagleDbTableData_New(table);
    
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

CUNIT_TEST(DBSuite, _, TableTest)
{
    // define the table
    EagleDbTable *table = _getTable();
    
    // create the data provider
    EagleDbTableData *td = EagleDbTableData_New(table);
    
    /*
     INSERT INTO mytable (col1, col2) VALUES (123, 456);
     */
    
    // create a record
    EagleDbTuple *tuple = EagleDbTuple_New(table);
    EagleDbTuple_setInt(tuple, 0, 123);
    EagleDbTuple_setText(tuple, 1, "hello");
    
    // put some data in
    EagleDbTableData_insert(td, tuple);
    CUNIT_ASSERT_EQUAL_INT(td->providers[0]->totalRecords, 1);
    CUNIT_ASSERT_EQUAL_INT(td->providers[1]->totalRecords, 1);
    CUNIT_ASSERT_EQUAL_INT(EaglePageProvider_pagesRemaining(td->providers[0]), 1);
    CUNIT_ASSERT_EQUAL_INT(EaglePageProvider_pagesRemaining(td->providers[1]), 1);
    
    /*
     SELECT col1, col2 FROM mytable;
     */
    
    // read the data out
    CUNIT_ASSERT_EQUAL_INT(td->providers[0]->type, EagleDataTypeInteger);
    EaglePage *page1 = EaglePageProvider_nextPage(td->providers[0]);
    CUNIT_ASSERT_EQUAL_INT(page1->type, EagleDataTypeInteger);
    
    CUNIT_ASSERT_EQUAL_INT(td->providers[1]->type, EagleDataTypeText);
    EaglePage *page2 = EaglePageProvider_nextPage(td->providers[1]);
    CUNIT_ASSERT_EQUAL_INT(page2->type, EagleDataTypeText);
    
    CUNIT_ASSERT_NOT_NULL(page1);
    CUNIT_ASSERT_NOT_NULL(page2);
    CUNIT_ASSERT_EQUAL_INT(page1->count, 1);
    CUNIT_ASSERT_EQUAL_INT(page2->count, 1);
    
    CUNIT_ASSERT_EQUAL_INT(((int*) page1->data)[0], 123);
    CUNIT_ASSERT_EQUAL_STRING(((char**) page2->data)[0], "hello");
    
    EagleDbTuple_Delete(tuple);
    EagleDbTableData_Delete(td);
    EagleDbTable_Delete(table);
    EaglePage_Delete(page1);
    EaglePage_Delete(page2);
}

CUNIT_TEST(DBSuite, EagleDbTuple_New)
{
    EagleDbTable *table = _getTable();
    
    EagleDbTuple *tuple = EagleDbTuple_New(table);
    EagleDbTuple_setInt(tuple, 0, 123);
    EagleDbTuple_setText(tuple, 1, "hello");
    char *desc = EagleDbTuple_toString(tuple);
    CUNIT_ASSERT_EQUAL_STRING(desc, "(col1=123,col2=\"hello\")");
    free(desc);
    
    EagleDbTuple_Delete(tuple);
    EagleDbTable_Delete(table);
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
    
    // the providers will contain the result
    plan->resultFields = 2;
    plan->result = (EaglePageProvider**) calloc((size_t) plan->resultFields, sizeof(EaglePageProvider*));
    plan->result[0] = EaglePageProvider_CreateFromStream(EagleDataTypeInteger, pageSize, "col1");
    plan->result[1] = EaglePageProvider_CreateFromStream(EagleDataTypeInteger, pageSize, "col2 + 8");
    
    // create a virtual table that consists of 2 columns; col1 and col2
    int *col1Data = (int*) calloc((size_t) pageSize, sizeof(int));
    int *col2Data = (int*) calloc((size_t) pageSize, sizeof(int));
    for(int i = 0; i < pageSize; ++i) {
        col1Data[i] = i;
        col2Data[i] = i * 2;
    }
    EaglePageProvider *col1 = EaglePageProvider_CreateFromIntArray(col1Data, pageSize, pageSize, "col1");
    EaglePageProvider *col2 = EaglePageProvider_CreateFromIntArray(col2Data, pageSize, pageSize, "col2");
    EaglePlan_addBufferProvider(plan, EaglePlanBufferProvider_New(1, col1, EagleTrue));
    EaglePlan_addBufferProvider(plan, EaglePlanBufferProvider_New(2, col2, EagleTrue));
    
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
    
    for(int i = 0; i < plan->resultFields; ++i) {
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
    
    for(int i = 0; i < exprs; ++i) {
        EagleDbSqlExpression_Delete(expr[i]);
    }
    for(int i = 0; i < plan->resultFields; ++i) {
        free(answers[i]);
    }
    free(answers_0);
    free(answers_1);
    free(answers);
    free(expr);
    EagleInstance_Delete(eagle);
    yylex_free();
}

CUNIT_TEST(DBSuite, _, SELECT_WHERE)
{
    if(_testSqlSelect("SELECT * FROM mytable WHERE 123")) {
        CUNIT_FAIL(yyerrors_last(), NULL);
    }
    
    EagleDbSqlSelect *select = (EagleDbSqlSelect*) yyparse_ast;
    CUNIT_ASSERT_EQUAL_STRING("mytable", select->tableName);
    CUNIT_ASSERT_NOT_NULL(select->whereExpression);
    CUNIT_ASSERT_EQUAL_INT(select->whereExpression->expressionType, EagleDbSqlExpressionTypeValue);
    
    EagleDbSqlValue *value = (EagleDbSqlValue*) select->whereExpression;
    CUNIT_ASSERT_EQUAL_INT(123, value->value.intValue);
    
    EagleDbSqlSelect_Delete(select);
    yylex_free();
}

CUNIT_TEST(DBSuite, _, CREATE_TABLE)
{
    if(_testSqlSelect("CREATE TABLE mytable ( col1 INT, col2 INTEGER )")) {
        CUNIT_FAIL(yyerrors_last(), NULL);
    }
    
    EagleDbTable *table = (EagleDbTable*) yyparse_ast;
    CUNIT_VERIFY_EQUAL_STRING("mytable", table->name);
    
    CUNIT_ASSERT_EQUAL_INT(table->allocatedColumns, 2);
    CUNIT_ASSERT_EQUAL_INT(table->usedColumns, 2);
    CUNIT_ASSERT_NOT_NULL(table->columns);
    
    CUNIT_ASSERT_EQUAL_STRING(table->columns[0]->name, "col1");
    CUNIT_ASSERT_EQUAL_INT(table->columns[0]->type, EagleDataTypeInteger);
    
    CUNIT_ASSERT_EQUAL_STRING(table->columns[1]->name, "col2");
    CUNIT_ASSERT_EQUAL_INT(table->columns[1]->type, EagleDataTypeInteger);
    
    EagleDbTable_Delete(table);
    yylex_free();
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

CUnitTests* DBSuite_tests()
{
    CUnitTests *tests = CUnitTests_New(100);
    
    // method tests
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbColumn_New));
    
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlBinaryExpression_New));
    
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlExpression_CompilePlan));
    
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlSelect_New));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlSelect_Delete));
    
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlValue_NewWithInteger));
    
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbTable_New));
    
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbTuple_New));
    
    // complex / execution tests
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _, BLANK));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _, SELECT_WHERE));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _, CREATE_TABLE));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _, TableTest));
    
    return tests;
}
