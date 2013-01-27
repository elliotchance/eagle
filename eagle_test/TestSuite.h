#ifndef eagle_TestSuite_h
#define eagle_TestSuite_h

#include <CUnit/Basic.h>
#include "stdarg.h"

#define CREATE_EXPRESSION_ARRAY(answers, pageSize, expr) \
int *answers = (int*) calloc((size_t) pageSize, sizeof(int)); \
for(int i = 0; i < pageSize; ++i) { \
answers[i] = expr; \
}

#define CUNIT_TESTNAME(suite, method, ...) suite##_test##method##__VA_ARGS__
#define CUNIT_TEST(suite, method, ...) void CUNIT_TESTNAME(suite, method, __VA_ARGS__)()
#define QUOTE(str) #str
#define CUNIT_NEW(suite, method, ...) CUnitTest_New(QUOTE(method##__VA_ARGS__), CUNIT_TESTNAME(suite, method, __VA_ARGS__))

/**
 Verify two integers are the same.
 */
#define CUNIT_VERIFY_EQUAL_INT(actual, expected) { \
char *cu_msg = (char*) malloc(1024); \
sprintf(cu_msg, "CUNIT_VERIFY_EQUAL_INT: Expected %d from expression \"%s\", but received %d from expression \"%s\"", expected, #expected, actual, #actual); \
if(CU_assertImplementation(((actual) == (expected)), __LINE__, cu_msg, __FILE__, "", CU_FALSE)) { \
free(cu_msg); \
}\
}

/**
 Assert two integers are the same.
 */
#define CUNIT_ASSERT_EQUAL_INT(actual, expected) { \
char *cu_msg = (char*) malloc(1024); \
sprintf(cu_msg, "CUNIT_ASSERT_EQUAL_INT: Expected %d from expression \"%s\", but received %d from expression \"%s\"", expected, #expected, actual, #actual); \
if(CU_assertImplementation(((actual) == (expected)), __LINE__, cu_msg, __FILE__, "", CU_TRUE)) { \
free(cu_msg); \
}\
}

/**
 Verify two doubles are the same.
 */
#define CUNIT_VERIFY_EQUAL_DOUBLE(actual, expected) { \
char *cu_msg = (char*) malloc(1024); \
sprintf(cu_msg, "CUNIT_VERIFY_EQUAL_DOUBLE: Expected %g from expression \"%s\", but received %g from expression \"%s\"", (double) (expected), #expected, (double) (actual), #actual); \
if(CU_assertImplementation(((actual) == (expected)), __LINE__, cu_msg, __FILE__, "", CU_FALSE)) { \
free(cu_msg); \
}\
}

/**
 Assert two doubles are the same.
 */
#define CUNIT_ASSERT_EQUAL_DOUBLE(actual, expected) { \
char *cu_msg = (char*) malloc(1024); \
sprintf(cu_msg, "CUNIT_ASSERT_EQUAL_DOUBLE: Expected %g from expression \"%s\", but received %g from expression \"%s\"", (double) (expected), #expected, (double) (actual), #actual); \
if(CU_assertImplementation(((actual) == (expected)), __LINE__, cu_msg, __FILE__, "", CU_TRUE)) { \
free(cu_msg); \
}\
}

/**
 Assert two strings are the same.
 */
#define CUNIT_VERIFY_EQUAL_STRING(actual, expected) { \
char *cu_msg = (char*) malloc(1024); \
sprintf(cu_msg, "CUNIT_VERIFY_EQUAL_STRING: Expected \"%s\", but received \"%s\"", expected, actual); \
if(CU_assertImplementation(!strcmp((actual), (expected)), __LINE__, cu_msg, __FILE__, "", CU_FALSE)) { \
free(cu_msg); \
}\
}

/**
 Assert two strings are the same.
 */
#define CUNIT_ASSERT_EQUAL_STRING(actual, expected) { \
char *cu_msg = (char*) malloc(1024); \
sprintf(cu_msg, "CUNIT_ASSERT_EQUAL_STRING: Expected \"%s\", but received \"%s\"", expected, actual); \
if(CU_assertImplementation(!strcmp((actual), (expected)), __LINE__, cu_msg, __FILE__, "", CU_TRUE)) { \
free(cu_msg); \
}\
}

/**
 Mark the test as failed.
 */
#define CUNIT_FAIL(msg, ...) { \
char *cu_msg = (char*) malloc(1024); \
sprintf(cu_msg, msg, __VA_ARGS__); \
if(CU_assertImplementation(CU_FALSE, __LINE__, cu_msg, __FILE__, "", CU_TRUE)) { \
free(cu_msg); \
}\
}

/**
 Assert a pointer is not NULL.
 */
#define CUNIT_ASSERT_NOT_NULL(actual) { \
char *cu_msg = (char*) malloc(1024); \
sprintf(cu_msg, "CUNIT_ASSERT_NOT_NULL: Expression \"%s\" was NULL", #actual); \
if(CU_assertImplementation((actual) != NULL, __LINE__, cu_msg, __FILE__, "", CU_TRUE)) { \
free(cu_msg); \
}\
}

/**
 Verify a pointer is not NULL.
 */
#define CUNIT_VERIFY_NOT_NULL(actual) { \
char *cu_msg = (char*) malloc(1024); \
sprintf(cu_msg, "CUNIT_VERIFY_NOT_NULL: Expression \"%s\" was NULL", #actual); \
if(CU_assertImplementation((actual) != NULL, __LINE__, cu_msg, __FILE__, "", CU_FALSE)) { \
free(cu_msg); \
}\
}

/**
 Assert a pointer is NULL.
 */
#define CUNIT_ASSERT_NULL(actual) { \
char *cu_msg = (char*) malloc(1024); \
sprintf(cu_msg, "CUNIT_ASSERT_NULL: Expression \"%s\" was not NULL", #actual); \
if(CU_assertImplementation((actual) == NULL, __LINE__, cu_msg, __FILE__, "", CU_TRUE)) { \
free(cu_msg); \
}\
}

/**
 Verify a pointer is NULL.
 */
#define CUNIT_VERIFY_NULL(actual) { \
char *cu_msg = (char*) malloc(1024); \
sprintf(cu_msg, "CUNIT_VERIFY_NULL: Expression \"%s\" was not NULL", #actual); \
if(CU_assertImplementation((actual) == NULL, __LINE__, cu_msg, __FILE__, "", CU_FALSE)) { \
free(cu_msg); \
}\
}

/**
 Assert two pointers are the same.
 */
#define CUNIT_ASSERT_EQUAL_PTR(actual, expected) { \
char *cu_msg = (char*) malloc(1024); \
sprintf(cu_msg, "CUNIT_ASSERT_EQUAL_PTR: Expected pointer from expression \"%s\" does not match pointer from expression \"%s\"", #expected, #actual); \
if(CU_assertImplementation(((actual) == (expected)), __LINE__, cu_msg, __FILE__, "", CU_TRUE)) { \
free(cu_msg); \
}\
}

/**
 Verify two pointers are the same.
 */
#define CUNIT_VERIFY_EQUAL_PTR(actual, expected) { \
char *cu_msg = (char*) malloc(1024); \
sprintf(cu_msg, "CUNIT_VERIFY_EQUAL_PTR: Expected pointer from expression \"%s\" does not match pointer from expression \"%s\"", #expected, #actual); \
if(CU_assertImplementation(((actual) == (expected)), __LINE__, cu_msg, __FILE__, "", CU_FALSE)) { \
free(cu_msg); \
}\
}

/**
 Verify one number is less than another.
 */
#define CUNIT_VERIFY_LESS_THAN_DOUBLE(x, y) { \
char *cu_msg = (char*) malloc(1024); \
sprintf(cu_msg, "CUNIT_VERIFY_LESS_THAN_DOUBLE: %f from expression \"%s\" is not less than %f from expression \"%s\"", x, #x, y, #y); \
if(CU_assertImplementation(((x) < (y)), __LINE__, cu_msg, __FILE__, "", CU_FALSE)) { \
free(cu_msg); \
}\
}

/**
 Assert one number is less than another.
 */
#define CUNIT_ASSERT_LESS_THAN_DOUBLE(x, y) { \
char *cu_msg = (char*) malloc(1024); \
sprintf(cu_msg, "CUNIT_ASSERT_LESS_THAN_DOUBLE: %f from expression \"%s\" is not less than %f from expression \"%s\"", x, #x, y, #y); \
if(CU_assertImplementation(((x) < (y)), __LINE__, cu_msg, __FILE__, "", CU_TRUE)) { \
free(cu_msg); \
}\
}

#define CUNIT_ASSERT_LAST_ERROR(msg) { EagleLoggerEvent *event = EagleLogger_LastEvent(); CUNIT_ASSERT_NOT_NULL(event); if(NULL != event) { CUNIT_ASSERT_EQUAL_STRING(event->message, msg); } }

typedef struct {
    const char *strName;
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
