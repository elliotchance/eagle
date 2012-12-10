#include <stdio.h>
#include <stdlib.h>
#include "DBSuite.h"
#include "EagleDbSqlSelect.h"

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
    //CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EaglePageOperations_LessThanInt));
    
    // complex / execution tests
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _, BLANK));
    
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _, SELECT_Simple));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _, SELECT_MissingTableName));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _, SELECT_MissingFROM));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _, SELECT_MissingFields));
    
    return tests;
}
