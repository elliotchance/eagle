#include "SQLFuzzSuite.h"
#include <stdio.h>
#include <stdlib.h>

CUNIT_TEST(SQLFuzzSuite, All)
{
}

CUnitTests* SQLFuzzSuite_tests()
{
    CUnitTests *tests = CUnitTests_New(100);
    
    // method tests
    CUnitTests_addTest(tests, CUNIT_NEW(SQLFuzzSuite, All));
    
    return tests;
}

/**
 * The suite init function.
 */
int SQLFuzzSuite_init()
{
    return 0;
}

/**
 * The suite cleanup function.
 */
int SQLFuzzSuite_clean()
{
    return 0;
}
