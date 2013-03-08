#include "MemorySuite.h"
#include <stdio.h>
#include <stdlib.h>
#include "EagleMemory.h"
#include "EagleDbSqlFunctionExpression.h"

CUNIT_TEST(MemorySuite, EagleDbSqlFunctionExpression_New)
{
    EagleMemory_MockInit();
    EagleMemory_Mock("EagleDbSqlFunctionExpression_New.1");
    
    CUNIT_VERIFY_NULL(EagleDbSqlFunctionExpression_New(NULL, NULL));
    
    CUNIT_ASSERT_EQUAL_INT(EagleMemory_GetMockInvocations(), 1);
    EagleMemory_MockFinish();
}

CUNIT_TEST(MemorySuite, EagleDbSqlFunctionExpression_toString)
{
    EagleMemory_MockInit();
    EagleMemory_Mock("EagleDbSqlFunctionExpression_toString.1");
    
    CUNIT_VERIFY_NULL(EagleDbSqlFunctionExpression_toString(NULL));
    
    CUNIT_ASSERT_EQUAL_INT(EagleMemory_GetMockInvocations(), 1);
    EagleMemory_MockFinish();
}

CUnitTests* MemorySuite2_tests()
{
    CUnitTests *tests = CUnitTests_New(100);
    
    // method tests
    CUnitTests_addTest(tests, CUNIT_NEW(MemorySuite, EagleDbSqlFunctionExpression_New));
    CUnitTests_addTest(tests, CUNIT_NEW(MemorySuite, EagleDbSqlFunctionExpression_toString));
    
    return tests;
}
