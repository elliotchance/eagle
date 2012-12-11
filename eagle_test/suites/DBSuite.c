#include <stdio.h>
#include <stdlib.h>
#include "DBSuite.h"
#include "EagleDbSqlSelect.h"
#include "EagleDbSqlBinaryExpression.h"
#include "EagleDbSqlValue.h"

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
        cunit_fail(yyerror_last);
    }
    yylex_destroy();
}

CUNIT_TEST(DBSuite, _, SELECT_Simple)
{
    // table name 1
    {
        if(_testSqlSelect("SELECT * FROM mytable1")) {
            cunit_fail(yyerror_last);
        }
        EagleDbSqlSelect *select = (EagleDbSqlSelect*) yyparse_ast;
        cunit_assert_equal_string("mytable1", select->tableName);
        yylex_destroy();
    }
    
    // table name 2
    {
        if(_testSqlSelect("SELECT * FROM mytable2")) {
            cunit_fail(yyerror_last);
        }
        EagleDbSqlSelect *select = (EagleDbSqlSelect*) yyparse_ast;
        cunit_assert_equal_string("mytable2", select->tableName);
        yylex_destroy();
    }
}

CUNIT_TEST(DBSuite, _, SELECT_MissingTableName)
{
    if(!_testSqlSelect("SELECT * FROM")) {
        cunit_fail("should have failed!");
    }
    cunit_assert_equal_string("syntax error, unexpected $end, expecting IDENTIFIER", yyerror_last);
    yylex_destroy();
}

CUNIT_TEST(DBSuite, _, SELECT_MissingFROM)
{
    if(!_testSqlSelect("SELECT *")) {
        cunit_fail("should have failed!");
    }
    cunit_assert_equal_string("syntax error, unexpected $end, expecting K_FROM", yyerror_last);
    yylex_destroy();
}

CUNIT_TEST(DBSuite, _, SELECT_MissingFields)
{
    if(!_testSqlSelect("SELECT")) {
        cunit_fail("should have failed!");
    }
    cunit_assert_equal_string("syntax error, unexpected $end, expecting T_ASTERISK", yyerror_last);
    yylex_destroy();
}

CUNIT_TEST(DBSuite, _, SELECT_WHERE_Integer)
{
    if(_testSqlSelect("SELECT * FROM mytable WHERE 123")) {
        cunit_fail(yyerror_last);
    }
    
    EagleDbSqlSelect *select = (EagleDbSqlSelect*) yyparse_ast;
    cunit_assert_equal_string("mytable", select->tableName);
    cunit_assert(NULL != select->whereExpression, "NULL != select->whereExpression");
    cunit_assert_equal_int(EagleDbSqlValueTypeInteger, select->whereExpression->type);
    cunit_assert_equal_int(123, select->whereExpression->value.intValue);
    
    yylex_destroy();
}

CUNIT_TEST(DBSuite, EagleDbSqlSelect_New)
{
    EagleDbSqlSelect *select = EagleDbSqlSelect_New();
    CU_ASSERT_NOT_EQUAL_FATAL(select, NULL);
    CU_ASSERT_EQUAL_FATAL(select->expressionType, EagleDbSqlExpressionTypeSelect);
    
    CU_ASSERT_EQUAL(select->tableName, NULL);
    CU_ASSERT_EQUAL(select->whereExpression, NULL);
}

CUNIT_TEST(DBSuite, EagleDbSqlBinaryExpression_New)
{
    EagleDbSqlExpression *left = (EagleDbSqlExpression*) EagleDbSqlValue_NewWithInteger(123);
    EagleDbSqlExpression *right = (EagleDbSqlExpression*) EagleDbSqlValue_NewWithInteger(456);
    
    EagleDbSqlBinaryExpression *binary = EagleDbSqlBinaryExpression_New(left, EagleDbSqlExpressionOperatorPlus, right);
    CU_ASSERT_NOT_EQUAL_FATAL(binary, NULL);
    CU_ASSERT_EQUAL_FATAL(binary->expressionType, EagleDbSqlExpressionTypeBinaryExpression);
    
    CU_ASSERT_PTR_EQUAL(binary->left, left);
    CU_ASSERT_EQUAL(binary->op, EagleDbSqlExpressionOperatorPlus);
    CU_ASSERT_PTR_EQUAL(binary->right, right);
}

CUNIT_TEST(DBSuite, EagleDbSqlValue_NewWithInteger)
{
    EagleDbSqlValue *value = EagleDbSqlValue_NewWithInteger(123);
    CU_ASSERT_NOT_EQUAL_FATAL(value, NULL);
    CU_ASSERT_EQUAL_FATAL(value->expressionType, EagleDbSqlExpressionTypeValue);
    
    CU_ASSERT_EQUAL_FATAL(value->type, EagleDbSqlValueTypeInteger);
    CU_ASSERT_EQUAL_FATAL(value->value.intValue, 123);
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
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _, SELECT_WHERE_Integer));
    
    return tests;
}
