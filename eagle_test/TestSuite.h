#ifndef eagle_TestSuite_h
#define eagle_TestSuite_h

#include <CUnit/Basic.h>
#include "stdarg.h"

#define CUNIT_TESTNAME(suite, method, ...) suite##_test##method##__VA_ARGS__
#define CUNIT_TEST(suite, method, ...) void CUNIT_TESTNAME(suite, method, __VA_ARGS__)()
#define QUOTE(str) #str
#define CUNIT_NEW(suite, method, ...) CUnitTest_New(QUOTE(suite##_test##method##__VA_ARGS__), CUNIT_TESTNAME(suite, method, __VA_ARGS__))

/**
 Verify two integers are the same.
 */
#define CUNIT_VERIFY_EQUAL_INT(actual, expected) { \
char *cu_msg = (char*) malloc(1024); \
sprintf(cu_msg, "CUNIT_VERIFY_EQUAL_INT: Expected %d from expression \"%s\", but received %d from expression \"%s\"", expected, #expected, actual, #actual); \
CU_assertImplementation(((actual) == (expected)), __LINE__, cu_msg, __FILE__, "", CU_FALSE); \
}

/**
 Assert two integers are the same.
 */
#define CUNIT_ASSERT_EQUAL_INT(actual, expected) { \
char *cu_msg = (char*) malloc(1024); \
sprintf(cu_msg, "CUNIT_ASSERT_EQUAL_INT: Expected %d from expression \"%s\", but received %d from expression \"%s\"", expected, #expected, actual, #actual); \
CU_assertImplementation(((actual) == (expected)), __LINE__, cu_msg, __FILE__, "", CU_TRUE); \
}

/**
 Assert two strings are the same.
 */
#define CUNIT_VERIFY_EQUAL_STRING(actual, expected) { \
char *cu_msg = (char*) malloc(1024); \
sprintf(cu_msg, "CUNIT_VERIFY_EQUAL_STRING: Expected \"%s\", but received \"%s\"", expected, actual); \
CU_assertImplementation(!strcmp((actual), (expected)), __LINE__, cu_msg, __FILE__, "", CU_FALSE); \
}

/**
 Assert two strings are the same.
 */
#define CUNIT_ASSERT_EQUAL_STRING(actual, expected) { \
char *cu_msg = (char*) malloc(1024); \
sprintf(cu_msg, "CUNIT_ASSERT_EQUAL_STRING: Expected \"%s\", but received \"%s\"", expected, actual); \
CU_assertImplementation(!strcmp((actual), (expected)), __LINE__, cu_msg, __FILE__, "", CU_TRUE); \
}

/**
 Mark the test as failed.
 */
#define CUNIT_FAIL(msg) { \
char *cu_msg = (char*) malloc(1024); \
sprintf(cu_msg, "CUNIT_FAIL: %s", msg); \
CU_assertImplementation(CU_FALSE, __LINE__, cu_msg, __FILE__, "", CU_TRUE); \
}

/**
 Assert a pointer is not NULL.
 */
#define CUNIT_ASSERT_NOT_NULL(actual) { \
char *cu_msg = (char*) malloc(1024); \
sprintf(cu_msg, "CUNIT_ASSERT_NOT_NULL: Expression \"%s\" was NULL", #actual); \
CU_assertImplementation((actual) != NULL, __LINE__, cu_msg, __FILE__, "", CU_TRUE); \
}

/**
 Verify a pointer is not NULL.
 */
#define CUNIT_VERIFY_NOT_NULL(actual) { \
char *cu_msg = (char*) malloc(1024); \
sprintf(cu_msg, "CUNIT_VERIFY_NOT_NULL: Expression \"%s\" was NULL", #actual); \
CU_assertImplementation((actual) != NULL, __LINE__, cu_msg, __FILE__, "", CU_FALSE); \
}

/**
 Assert a pointer is NULL.
 */
#define CUNIT_ASSERT_NULL(actual) { \
char *cu_msg = (char*) malloc(1024); \
sprintf(cu_msg, "CUNIT_ASSERT_NULL: Expression \"%s\" was not NULL", #actual); \
CU_assertImplementation((actual) == NULL, __LINE__, cu_msg, __FILE__, "", CU_TRUE); \
}

/**
 Verify a pointer is NULL.
 */
#define CUNIT_VERIFY_NULL(actual) { \
char *cu_msg = (char*) malloc(1024); \
sprintf(cu_msg, "CUNIT_VERIFY_NULL: Expression \"%s\" was not NULL", #actual); \
CU_assertImplementation((actual) == NULL, __LINE__, cu_msg, __FILE__, "", CU_FALSE); \
}

/**
 Assert two pointers are the same.
 */
#define CUNIT_ASSERT_EQUAL_PTR(actual, expected) { \
char *cu_msg = (char*) malloc(1024); \
sprintf(cu_msg, "CUNIT_ASSERT_EQUAL_PTR: Expected pointer from expression \"%s\" does not match pointer from expression \"%s\"", #expected, #actual); \
CU_assertImplementation(((actual) == (expected)), __LINE__, cu_msg, __FILE__, "", CU_TRUE); \
}

/**
 Verify two pointers are the same.
 */
#define CUNIT_VERIFY_EQUAL_PTR(actual, expected) { \
char *cu_msg = (char*) malloc(1024); \
sprintf(cu_msg, "CUNIT_VERIFY_EQUAL_PTR: Expected pointer from expression \"%s\" does not match pointer from expression \"%s\"", #expected, #actual); \
CU_assertImplementation(((actual) == (expected)), __LINE__, cu_msg, __FILE__, "", CU_FALSE); \
}


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
void CUnitTest_Delete(CUnitTest *obj);

CUnitTests* CUnitTests_New(int allocatedTests);
void CUnitTests_Delete(CUnitTests *obj);
void CUnitTests_addTest(CUnitTests *tests, CUnitTest *test);

#endif
