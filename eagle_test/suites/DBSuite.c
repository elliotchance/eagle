#include <stdio.h>
#include <stdlib.h>
#include "DBSuite.h"

CUNIT_TEST(DBSuite, _, Select)
{
    const char *sql = "15 + 7";
    
    yy_scan_string(sql);
    
    if(yyparse() == 0) {
        fprintf(stderr, "Successful parsing.\n");
    }
    else {
        fprintf(stderr, "error found.\n");
    }
    
    // clean
    //yylex_destroy();
    
    //cunit_assert(valid, "Not valid");
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
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _, Select));
    
    return tests;
}
