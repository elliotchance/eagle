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
    yylex_init();
    yy_scan_string(sql);
    return yyparse();
}

CUNIT_TEST(DBSuite, _, BLANK)
{
    if(_testSqlSelect("")) {
        CUNIT_FAIL(yyerrors_last(), NULL);
    }
    yylex_free();
}

CUNIT_TEST(DBSuite, _, SELECT_Simple)
{
    // table name 1
    {
        if(_testSqlSelect("SELECT * FROM mytable1")) {
            CUNIT_FAIL(yyerrors_last(), NULL);
        }
        EagleDbSqlSelect *select = (EagleDbSqlSelect*) yyparse_ast;
        CUNIT_ASSERT_EQUAL_STRING("mytable1", select->tableName);
        
        EagleDbSqlSelect_Delete(select);
        yylex_free();
    }
    
    // table name 2
    {
        if(_testSqlSelect("SELECT * FROM mytable2")) {
            CUNIT_FAIL(yyerrors_last(), NULL);
        }
        EagleDbSqlSelect *select = (EagleDbSqlSelect*) yyparse_ast;
        CUNIT_ASSERT_EQUAL_STRING("mytable2", select->tableName);
        
        EagleDbSqlSelect_Delete(select);
        yylex_free();
    }
}

CUNIT_TEST(DBSuite, _, SELECT_MissingTableName)
{
    if(!_testSqlSelect("SELECT * FROM")) {
        CUNIT_FAIL("should have failed!", NULL);
    }
    CUNIT_ASSERT_EQUAL_STRING(yyerrors_last(), "syntax error, unexpected $end, expecting IDENTIFIER");
    yylex_free();
}

CUNIT_TEST(DBSuite, _, SELECT_MissingFROM)
{
    if(!_testSqlSelect("SELECT *")) {
        CUNIT_FAIL("should have failed!", NULL);
    }
    CUNIT_ASSERT_EQUAL_STRING(yyerrors_last(), "syntax error, unexpected $end, expecting K_FROM");
    yylex_free();
}

CUNIT_TEST(DBSuite, _, SELECT_MissingFields)
{
    if(!_testSqlSelect("SELECT")) {
        CUNIT_FAIL("should have failed!", NULL);
    }
    CUNIT_ASSERT_EQUAL_STRING(yyerrors_last(), "syntax error, unexpected $end, expecting INTEGER or T_ASTERISK");
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
    CUNIT_ASSERT_NOT_NULL(select->selectExpression);
    
    return select->selectExpression;
}

void _testExpression(EagleDbSqlExpression *where, int usedProviders, int usedOperations, int *answers)
{
    // compile plan
    int pageSize = 10;
    EaglePageReceiver *dummy = EaglePageReceiver_New();
    EaglePageProvider *receiver = EaglePageProvider_CreateFromIntStream(pageSize);
    EaglePlan *plan = EaglePlan_New(pageSize, dummy);
    EagleDbSqlExpression_CompilePlanIntoProvider(where, receiver, plan);
    //printf("\n%s\n", EaglePlan_toString(plan));
    
    CUNIT_ASSERT_EQUAL_INT(plan->usedProviders, usedProviders);
    CUNIT_ASSERT_EQUAL_INT(plan->usedOperations, usedOperations);
    
    // execute
    EagleInstance *eagle = EagleInstance_New(1);
    EagleInstance_addPlan(eagle, plan);
    EagleInstance_run(eagle);
    
    // validate result
    CUNIT_ASSERT_EQUAL_INT(receiver->totalRecords, pageSize);
    int valid = 1;
    for(int i = 0; i < pageSize; ++i) {
        if(((int*) receiver->records)[i] != answers[i]) {
            valid = 0;
            break;
        }
    }
    CUNIT_ASSERT_EQUAL_INT(valid, 1);
    
    EaglePageProvider_Delete(receiver);
    EagleInstance_Delete(eagle);
}

CUNIT_TEST(DBSuite, _, Expression_ValueInteger)
{
    // SQL
    EagleDbSqlExpression *where = _getExpression("SELECT 123 FROM mytable");
    CUNIT_ASSERT_EQUAL_INT(where->expressionType, EagleDbSqlExpressionTypeValue);
    
    // AST
    EagleDbSqlValue *value = (EagleDbSqlValue*) where;
    CUNIT_ASSERT_EQUAL_INT(value->value.intValue, 123);
    
    CREATE_EXPRESSION_ARRAY(answers, 10, 123);
    _testExpression(where, 1, 1, answers);
    free(answers);
    
    EagleDbSqlSelect_Delete(yyparse_ast);
    yylex_free();
}

CUNIT_TEST(DBSuite, _, Expression_Addition)
{
    // SQL
    EagleDbSqlExpression *where = _getExpression("SELECT 123 + 456 FROM mytable");
    CUNIT_ASSERT_EQUAL_INT(EagleDbSqlExpressionTypeBinaryExpression, where->expressionType);
    
    // AST
    EagleDbSqlBinaryExpression *expr = (EagleDbSqlBinaryExpression*) where;
    CUNIT_ASSERT_EQUAL_INT(EagleDbSqlExpressionTypeValue, expr->left->expressionType);
    CUNIT_ASSERT_EQUAL_INT(EagleDbSqlExpressionTypeValue, expr->right->expressionType);
    
    EagleDbSqlValue *left = (EagleDbSqlValue*) expr->left;
    CUNIT_ASSERT_EQUAL_INT(EagleDbSqlValueTypeInteger, left->type);
    
    EagleDbSqlValue *right = (EagleDbSqlValue*) expr->right;
    CUNIT_ASSERT_EQUAL_INT(EagleDbSqlValueTypeInteger, right->type);
    
    CUNIT_ASSERT_EQUAL_INT(123, ((EagleDbSqlValue*) left)->value.intValue);
    CUNIT_ASSERT_EQUAL_INT(456, ((EagleDbSqlValue*) right)->value.intValue);
    
    CREATE_EXPRESSION_ARRAY(answers, 10, 123 + 456);
    _testExpression(where, 2, 2, answers);
    free(answers);
    
    EagleDbSqlSelect_Delete(yyparse_ast);
    yylex_free();
}

CUNIT_TEST(DBSuite, EagleDbColumn_New)
{
    EagleDbColumn *col = EagleDbColumn_New("mycol", EagleDbColumnTypeInteger);
    CUNIT_ASSERT_EQUAL_STRING(col->name, "mycol");
    CUNIT_ASSERT_EQUAL_INT(col->type, EagleDbColumnTypeInteger);
    EagleDbColumn_Delete(col);
}

CUNIT_TEST(DBSuite, EagleDbSqlSelect_Delete)
{
    EagleDbSqlSelect_Delete(NULL);
}

CUNIT_TEST(DBSuite, EagleDbTable_New)
{
    EagleDbTable *table = EagleDbTable_New("mytable");
    EagleDbTable_addColumn(table, EagleDbColumn_New("col1", EagleDbColumnTypeInteger));
    EagleDbTable_addColumn(table, EagleDbColumn_New("col2", EagleDbColumnTypeInteger));
    
    CUNIT_ASSERT_EQUAL_STRING(table->name, "mytable");
    CUNIT_ASSERT_EQUAL_INT(table->usedColumns, 2);
    CUNIT_ASSERT_EQUAL_STRING(table->columns[0]->name, "col1");
    CUNIT_ASSERT_EQUAL_INT(table->columns[0]->type, EagleDbColumnTypeInteger);
    CUNIT_ASSERT_EQUAL_STRING(table->columns[1]->name, "col2");
    CUNIT_ASSERT_EQUAL_INT(table->columns[1]->type, EagleDbColumnTypeInteger);
    
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
    EagleDbTable_addColumn(table, EagleDbColumn_New("col1", EagleDbColumnTypeInteger));
    EagleDbTable_addColumn(table, EagleDbColumn_New("col2", EagleDbColumnTypeInteger));
    
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
    EagleDbTuple_setInt(tuple, 1, 456);
    
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
    EaglePage *page1 = EaglePageProvider_nextPage(td->providers[0]);
    EaglePage *page2 = EaglePageProvider_nextPage(td->providers[1]);
    
    CUNIT_ASSERT_NOT_NULL(page1);
    CUNIT_ASSERT_NOT_NULL(page2);
    CUNIT_ASSERT_EQUAL_INT(page1->count, 1);
    CUNIT_ASSERT_EQUAL_INT(page2->count, 1);
    
    CUNIT_ASSERT_EQUAL_INT(page1->data[0], 123);
    CUNIT_ASSERT_EQUAL_INT(page2->data[0], 456);
    
    EaglePage_Delete(page1);
    EaglePage_Delete(page2);
    
    EagleDbTuple_Delete(tuple);
    EagleDbTableData_Delete(td);
    EagleDbTable_Delete(table);
}

CUNIT_TEST(DBSuite, EagleDbTuple_New)
{
    EagleDbTable *table = _getTable();
    
    EagleDbTuple *tuple = EagleDbTuple_New(table);
    EagleDbTuple_setInt(tuple, 0, 123);
    EagleDbTuple_setInt(tuple, 1, 456);
    char *desc = EagleDbTuple_toString(tuple);
    CUNIT_ASSERT_EQUAL_STRING(desc, "(col1=123,col2=456)");
    free(desc);
    
    EagleDbTuple_Delete(tuple);
    EagleDbTable_Delete(table);
}

CUNIT_TEST(DBSuite, EagleDbSqlExpression_CompilePlanIntoBoolean)
{
    EagleDbSqlExpression *where = _getExpression("SELECT 123 FROM mytable");
    
    // compile plan
    int pageSize = 10;
    EaglePageReceiver *receiver = EaglePageReceiver_New();
    EaglePlan *plan = EaglePlan_New(pageSize, receiver);
    EagleDbSqlExpression_CompilePlanIntoBoolean(where, 1, plan);
    //printf("\n%s\n", EaglePlan_toString(plan));
    
    CUNIT_ASSERT_EQUAL_INT(plan->usedProviders, 1);
    CUNIT_ASSERT_EQUAL_INT(plan->usedOperations, 1);
    
    // execute
    EagleInstance *eagle = EagleInstance_New(1);
    EagleInstance_addPlan(eagle, plan);
    EagleInstance_run(eagle);
    
    // validate result
    CUNIT_ASSERT_EQUAL_INT(receiver->used, pageSize);
    int valid = 1;
    for(int i = 0; i < pageSize; ++i) {
        if(receiver->buffer[i] != i) {
            valid = 0;
            break;
        }
    }
    CUNIT_ASSERT_EQUAL_INT(valid, 1);
    
    EagleInstance_Delete(eagle);
    EagleDbSqlSelect_Delete(yyparse_ast);
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
    
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlExpression_CompilePlanIntoBoolean));
    
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlSelect_New));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlSelect_Delete));
    
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlValue_NewWithInteger));
    
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbTable_New));
    
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbTuple_New));
    
    // complex / execution tests
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _, BLANK));
    
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _, SELECT_Simple));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _, SELECT_MissingTableName));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _, SELECT_MissingFROM));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _, SELECT_MissingFields));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _, SELECT_WHERE));
    
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _, Expression_ValueInteger));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _, Expression_Addition));
    
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _, TableTest));
    
    return tests;
}
