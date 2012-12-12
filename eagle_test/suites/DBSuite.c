#include <stdio.h>
#include <stdlib.h>
#include "DBSuite.h"
#include "EagleDbSqlSelect.h"
#include "EagleDbSqlBinaryExpression.h"
#include "EagleDbSqlValue.h"
#include "EagleInstance.h"
#include "EagleData.h"

extern void *yyparse_ast;
extern char *yyerror_last;
int yylex_destroy();

int _testSqlSelect(const char *sql)
{
    yy_scan_string(sql);
    return yyparse();
}

CUNIT_TEST(DBSuite, _, BLANK)
{
    if(_testSqlSelect("")) {
        CUNIT_FAIL(yyerror_last);
    }
    yylex_destroy();
}

CUNIT_TEST(DBSuite, _, SELECT_Simple)
{
    // table name 1
    {
        if(_testSqlSelect("SELECT * FROM mytable1")) {
            CUNIT_FAIL(yyerror_last);
        }
        EagleDbSqlSelect *select = (EagleDbSqlSelect*) yyparse_ast;
        CUNIT_ASSERT_EQUAL_STRING("mytable1", select->tableName);
        yylex_destroy();
    }
    
    // table name 2
    {
        if(_testSqlSelect("SELECT * FROM mytable2")) {
            CUNIT_FAIL(yyerror_last);
        }
        EagleDbSqlSelect *select = (EagleDbSqlSelect*) yyparse_ast;
        CUNIT_ASSERT_EQUAL_STRING("mytable2", select->tableName);
        yylex_destroy();
    }
}

CUNIT_TEST(DBSuite, _, SELECT_MissingTableName)
{
    if(!_testSqlSelect("SELECT * FROM")) {
        CUNIT_FAIL("should have failed!");
    }
    CUNIT_ASSERT_EQUAL_STRING(yyerror_last, "syntax error, unexpected $end, expecting IDENTIFIER");
    yylex_destroy();
}

CUNIT_TEST(DBSuite, _, SELECT_MissingFROM)
{
    if(!_testSqlSelect("SELECT *")) {
        CUNIT_FAIL("should have failed!");
    }
    CUNIT_ASSERT_EQUAL_STRING(yyerror_last, "syntax error, unexpected $end, expecting K_FROM");
    yylex_destroy();
}

CUNIT_TEST(DBSuite, _, SELECT_MissingFields)
{
    if(!_testSqlSelect("SELECT")) {
        CUNIT_FAIL("should have failed!");
    }
    CUNIT_ASSERT_EQUAL_STRING("syntax error, unexpected $end, expecting T_ASTERISK", yyerror_last);
    yylex_destroy();
}

CUNIT_TEST(DBSuite, _, SELECT_WHERE)
{
    if(_testSqlSelect("SELECT * FROM mytable WHERE 123")) {
        CUNIT_FAIL(yyerror_last);
    }
    
    EagleDbSqlSelect *select = (EagleDbSqlSelect*) yyparse_ast;
    CUNIT_ASSERT_EQUAL_STRING("mytable", select->tableName);
    CUNIT_ASSERT_NOT_NULL(select->whereExpression);
    CUNIT_ASSERT_EQUAL_INT(EagleDbSqlValueTypeInteger, select->whereExpression->expressionType);
    
    EagleDbSqlValue *value = (EagleDbSqlValue*) select->whereExpression;
    CUNIT_ASSERT_EQUAL_INT(123, value->value.intValue);
    
    yylex_destroy();
}

CUNIT_TEST(DBSuite, EagleDbSqlSelect_New)
{
    EagleDbSqlSelect *select = EagleDbSqlSelect_New();
    CUNIT_ASSERT_NOT_NULL(select);
    CUNIT_ASSERT_EQUAL_INT(select->expressionType, EagleDbSqlExpressionTypeSelect);
    
    CUNIT_ASSERT_NULL(select->tableName);
    CUNIT_ASSERT_NULL(select->whereExpression);
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
}

CUNIT_TEST(DBSuite, EagleDbSqlValue_NewWithInteger)
{
    EagleDbSqlValue *value = EagleDbSqlValue_NewWithInteger(123);
    CUNIT_ASSERT_NOT_NULL(value);
    CUNIT_ASSERT_EQUAL_INT(value->expressionType, EagleDbSqlExpressionTypeValue);
    
    CUNIT_ASSERT_EQUAL_INT(value->type, EagleDbSqlValueTypeInteger);
    CUNIT_ASSERT_EQUAL_INT(value->value.intValue, 123);
}

EagleDbSqlExpression* _getExpression(const char *sql)
{
    if(_testSqlSelect(sql)) {
        CUNIT_FAIL(yyerror_last);
    }
    
    EagleDbSqlSelect *select = (EagleDbSqlSelect*) yyparse_ast;
    CUNIT_ASSERT_NOT_NULL(select->whereExpression);
    
    return select->whereExpression;
}

void _testExpression(EagleDbSqlExpression *where, int usedProviders, int userOperations)
{
    // compile plan
    int pageSize = 10;
    EaglePageReceiver *receiver = EaglePageReceiver_New();
    EaglePlan *plan = EaglePlan_New(pageSize, receiver);
    EagleDbSqlExpression_CompilePlan(where, 1, plan, 0);
    //printf("%s\n", EaglePlan_toString(plan));
    
    CUNIT_ASSERT_EQUAL_INT(plan->usedProviders, usedProviders);
    CUNIT_ASSERT_EQUAL_INT(plan->usedOperations, userOperations);
    
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
    
    yylex_destroy();
}

CUNIT_TEST(DBSuite, _, Expression_ValueInteger)
{
    // SQL
    EagleDbSqlExpression *where = _getExpression("SELECT * FROM mytable WHERE 123");
    CUNIT_ASSERT_EQUAL_INT(EagleDbSqlValueTypeInteger, where->expressionType);
    
    // AST
    EagleDbSqlValue *value = (EagleDbSqlValue*) where;
    CUNIT_ASSERT_EQUAL_INT(123, value->value.intValue);
    
    _testExpression(where, 1, 1);
    
    yylex_destroy();
}

CUNIT_TEST(DBSuite, _, Expression_Addition)
{
    // SQL
    EagleDbSqlExpression *where = _getExpression("SELECT * FROM mytable WHERE 123 + 456");
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
    
    _testExpression(where, 2, 2);
    
    yylex_destroy();
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
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlBinaryExpression_New));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlSelect_New));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlValue_NewWithInteger));
    
    // complex / execution tests
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _, BLANK));
    
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _, SELECT_Simple));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _, SELECT_MissingTableName));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _, SELECT_MissingFROM));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _, SELECT_MissingFields));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _, SELECT_WHERE));
    
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _, Expression_ValueInteger));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _, Expression_Addition));
    
    return tests;
}
