#include <stdlib.h>
#include "TestSuite.h"

CUnitTest* CUnitTest_New(const char* strName, CU_TestFunc pTestFunc)
{
    CUnitTest *test = (CUnitTest*) malloc(sizeof(CUnitTest));
    test->strName = strName;
    test->pTestFunc = pTestFunc;
    return test;
}

CUnitTests* CUnitTests_New(int allocatedTests)
{
    CUnitTests *tests = (CUnitTests*) malloc(sizeof(CUnitTests));
    tests->allocatedTests = allocatedTests;
    tests->usedTests = 0;
    tests->tests = (CUnitTest**) calloc(tests->allocatedTests, sizeof(sizeof(CUnitTest)));
    return tests;
}

void CUnitTests_addTest(CUnitTests *tests, CUnitTest *test)
{
    tests->tests[tests->usedTests++] = test;
}

void cunit_verify(int test, const char *format, ...)
{
    if(test) {
        // everything is fine, register the assertion count
        CU_ASSERT(test);
    }
    else {
        // failed, generate the message
        char message[1024];
        va_list argptr;
        va_start(argptr, format);
        vsprintf(message, format, argptr);
        va_end(argptr);
        
        CU_ASSERT_FALSE(message);
    }
}

void cunit_verify_equal_int(int left, int right)
{
    if(left == right) {
        // everything is fine, register the assertion count
        CU_ASSERT(left == right);
    }
    else {
        // failed, generate the message
        char message[1024];
        sprintf(message, "%d == %d", left, right);
        CU_ASSERT_FALSE(message);
    }
}

void cunit_verify_equal_string(char *left, char *right)
{
    if(!strcmp(left, right)) {
        // everything is fine, register the assertion count
        CU_ASSERT_TRUE(1);
    }
    else {
        // failed, generate the message
        char message[1024];
        sprintf(message, "\"%s\" == \"%s\"", left, right);
        CU_ASSERT_FALSE(message);
    }
}

void cunit_assert(int test, const char *format, ...)
{
    if(test) {
        // everything is fine, register the assertion count
        CU_ASSERT(test);
    }
    else {
        // assert does not let the test continue
        char message[1024];
        va_list argptr;
        va_start(argptr, format);
        vsprintf(message, format, argptr);
        va_end(argptr);
        
        CU_ASSERT_FALSE_FATAL(message);
    }
}

void cunit_assert_equal_int(int left, int right)
{
    if(left == right) {
        // everything is fine, register the assertion count
        CU_ASSERT_FATAL(left == right);
    }
    else {
        // failed, generate the message
        char message[1024];
        sprintf(message, "%d == %d", left, right);
        CU_ASSERT_FALSE_FATAL(message);
    }
}

void cunit_assert_equal_string(char *left, char *right)
{
    if(!strcmp(left, right)) {
        // everything is fine, register the assertion count
        CU_ASSERT_TRUE(1);
    }
    else {
        // failed, generate the message
        char message[1024];
        sprintf(message, "\"%s\" == \"%s\"", left, right);
        CU_ASSERT_FALSE_FATAL(message);
    }
}
