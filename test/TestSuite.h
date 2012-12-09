#ifndef eagle_TestSuite_h
#define eagle_TestSuite_h

#include <CUnit/Basic.h>

#define CUNIT_TEST(suite, name) void suite##_##name()
#define CUNIT_NEW(suite, name) CUnitTest_New(#name, suite##_##name)

typedef struct {
    const char* strName;
    CU_TestFunc pTestFunc;
} CUnitTest;

typedef struct {
    int allocatedTests;
    int usedTests;
    CUnitTest **tests;
} CUnitTests;

CUnitTest* CUnitTest_New(const char* strName, CU_TestFunc pTestFunc);

CUnitTests* CUnitTests_New(int allocatedTests);
void CUnitTests_addTest(CUnitTests *tests, CUnitTest *test);

#endif
