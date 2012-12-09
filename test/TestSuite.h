#ifndef eagle_TestSuite_h
#define eagle_TestSuite_h

#include <CUnit/Basic.h>

#define CUNIT_TESTNAME(suite, method, ...) suite##_test##method##__VA_ARGS__
#define CUNIT_TEST(suite, method, ...) void CUNIT_TESTNAME(suite, method, __VA_ARGS__)()
#define QUOTE(str) #str
#define CUNIT_NEW(suite, method, ...) CUnitTest_New(QUOTE(suite##_test##method##__VA_ARGS__), CUNIT_TESTNAME(suite, method, __VA_ARGS__))

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
