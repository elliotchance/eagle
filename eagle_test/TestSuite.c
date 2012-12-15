#include <stdlib.h>
#include "TestSuite.h"

CUnitTest* CUnitTest_New(const char *strName, CU_TestFunc pTestFunc)
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
    tests->tests = (CUnitTest**) calloc(tests->allocatedTests, sizeof(CUnitTest*));
    return tests;
}

void CUnitTests_addTest(CUnitTests *tests, CUnitTest *test)
{
    tests->tests[tests->usedTests++] = test;
}

void CUnitTest_Delete(CUnitTest *obj)
{
    free((void*) obj);
}

void CUnitTests_Delete(CUnitTests *obj)
{
    for(int i = 0; i < obj->usedTests; ++i) {
        CUnitTest_Delete(obj->tests[i]);
    }
    free((void*) obj->tests);
    free((void*) obj);
}
