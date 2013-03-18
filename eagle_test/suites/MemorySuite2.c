#include "MemorySuite.h"
#include <stdio.h>
#include <stdlib.h>
#include "EagleMemory.h"
#include "EagleDbSqlFunctionExpression.h"
#include "EaglePageProviderSingle.h"
#include "EagleDbSqlValue.h"
#include "EagleDbSqlCastExpression.h"

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

CUNIT_TEST(MemorySuite, EagleData_Float)
{
    EagleMemory_MockInit();
    EagleMemory_Mock("EagleData_Float.1");
    
    CUNIT_VERIFY_NULL(EagleData_Float(123.0));
    
    CUNIT_ASSERT_EQUAL_INT(EagleMemory_GetMockInvocations(), 1);
    EagleMemory_MockFinish();
}

CUNIT_TEST(MemorySuite, EagleDbSqlValue_NewWithFloat)
{
    EagleMemory_MockInit();
    EagleMemory_Mock("EagleDbSqlValue_NewWithFloat.1");
    
    CUNIT_VERIFY_NULL(EagleDbSqlValue_NewWithFloat(123.0));
    
    CUNIT_ASSERT_EQUAL_INT(EagleMemory_GetMockInvocations(), 1);
    EagleMemory_MockFinish();
}

CUNIT_TEST(MemorySuite, EaglePageProviderSingle_NewFloat)
{
    EagleMemory_MockInit();
    EagleMemory_Mock("EaglePageProviderSingle_NewFloat.1");
    
    CUNIT_VERIFY_NULL(EaglePageProviderSingle_NewFloat(123.0, 1, "name"));
    
    CUNIT_ASSERT_EQUAL_INT(EagleMemory_GetMockInvocations(), 1);
    EagleMemory_MockFinish();
}

CUNIT_TEST(MemorySuite, EaglePageProviderSingle_nextPage_3)
{
    EagleMemory_MockInit();
    EagleMemory_Mock("EaglePageProviderSingle_nextPage.3");
    
    EaglePageProviderSingle *single = EaglePageProviderSingle_NewFloat(123.0, 1, "name");
    CUNIT_VERIFY_NULL(EaglePageProviderSingle_nextPage(single));
    EaglePageProviderSingle_Delete(single);
    
    CUNIT_ASSERT_EQUAL_INT(EagleMemory_GetMockInvocations(), 1);
    EagleMemory_MockFinish();
}

CUNIT_TEST(MemorySuite, EaglePage_AllocFloat)
{
    EagleMemory_MockInit();
    EagleMemory_Mock("EaglePage_AllocFloat.1");
    
    CUNIT_VERIFY_NULL(EaglePage_AllocFloat(1));
    
    CUNIT_ASSERT_EQUAL_INT(EagleMemory_GetMockInvocations(), 1);
    EagleMemory_MockFinish();
}

CUNIT_TEST(MemorySuite, EaglePage_CopyFloat_)
{
    EagleMemory_MockInit();
    EagleMemory_Mock("EaglePage_CopyFloat_.1");
    
    EaglePage *page = EaglePage_AllocFloat(1);
    CUNIT_VERIFY_NULL(EaglePage_CopyFloat_(page));
    EaglePage_Delete(page);
    
    CUNIT_ASSERT_EQUAL_INT(EagleMemory_GetMockInvocations(), 1);
    EagleMemory_MockFinish();
}

CUNIT_TEST(MemorySuite, EagleDbSqlCastExpression_New)
{
    EagleMemory_MockInit();
    EagleMemory_Mock("EagleDbSqlCastExpression_New.1");
    
    CUNIT_VERIFY_NULL(EagleDbSqlCastExpression_New(NULL, EagleDataTypeInteger));
    
    CUNIT_ASSERT_EQUAL_INT(EagleMemory_GetMockInvocations(), 1);
    EagleMemory_MockFinish();
}

CUNIT_TEST(MemorySuite, EagleDbSqlCastExpression_toString)
{
    EagleMemory_MockInit();
    EagleMemory_Mock("EagleDbSqlCastExpression_toString.1");
    
    CUNIT_VERIFY_NULL(EagleDbSqlCastExpression_toString(NULL));
    
    CUNIT_ASSERT_EQUAL_INT(EagleMemory_GetMockInvocations(), 1);
    EagleMemory_MockFinish();
}

CUNIT_TEST(MemorySuite, EaglePlanBufferProvider_NewWithValue)
{
    EagleMemory_MockInit();
    EagleMemory_Mock("EaglePlanBufferProvider_NewWithValue.1");
    
    CUNIT_VERIFY_NULL(EaglePlanBufferProvider_NewWithValue(0, NULL));
    
    CUNIT_ASSERT_EQUAL_INT(EagleMemory_GetMockInvocations(), 1);
    EagleMemory_MockFinish();
}

CUnitTests* MemorySuite2_tests()
{
    CUnitTests *tests = CUnitTests_New(100);
    
    // method tests
    CUnitTests_addTest(tests, CUNIT_NEW(MemorySuite, EaglePlanBufferProvider_NewWithValue));
    CUnitTests_addTest(tests, CUNIT_NEW(MemorySuite, EagleDbSqlCastExpression_toString));
    CUnitTests_addTest(tests, CUNIT_NEW(MemorySuite, EagleDbSqlCastExpression_New));
    CUnitTests_addTest(tests, CUNIT_NEW(MemorySuite, EagleDbSqlFunctionExpression_New));
    CUnitTests_addTest(tests, CUNIT_NEW(MemorySuite, EagleDbSqlFunctionExpression_toString));
    CUnitTests_addTest(tests, CUNIT_NEW(MemorySuite, EagleData_Float));
    CUnitTests_addTest(tests, CUNIT_NEW(MemorySuite, EagleDbSqlValue_NewWithFloat));
    CUnitTests_addTest(tests, CUNIT_NEW(MemorySuite, EaglePageProviderSingle_NewFloat));
    CUnitTests_addTest(tests, CUNIT_NEW(MemorySuite, EaglePageProviderSingle_nextPage_3));
    CUnitTests_addTest(tests, CUNIT_NEW(MemorySuite, EaglePage_AllocFloat));
    CUnitTests_addTest(tests, CUNIT_NEW(MemorySuite, EaglePage_CopyFloat_));
    
    return tests;
}
