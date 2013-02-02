#include "TestSuite.h"
#include <CUnit/Basic.h>
#include <stdio.h>
#include <unistd.h>
#include "MainSuite.h"
#include "DBSuite.h"
#include "SQLSuite.h"
#include "MemorySuite.h"
#include "BenchSuite.h"
#include "EagleLogger.h"
#include "SQLFuzzSuite.h"

CU_ErrorCode addSuite(const char *name, CU_InitializeFunc pInit, CU_CleanupFunc pClean, CUnitTests* (*testsFunction)())
{
    // add a suite to the registry
    CU_pSuite pSuite = CU_add_suite(name, pInit, pClean);
    if(NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    // add the tests to the suite
    CUnitTests *tests = testsFunction();
    for(int i = 0; i < tests->usedTests; ++i) {
        CUnitTest *test = tests->tests[i];
        if(NULL == CU_add_test(pSuite, test->strName, test->pTestFunc)) {
            CU_cleanup_registry();
            return CU_get_error();
        }
    }
    
    // clean up
    CUnitTests_Delete(tests);
    return CUE_SUCCESS;
}

/**
 * The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another CUnit error code on failure.
 */
int main(int argc, char **argv)
{
    // if we are checking for leaks then this may need to wait
    int wait = 0;
    if(argc > 1 && !strcmp(argv[1], "wait")) {
        wait = 1;
    }
    
    // initialize the CUnit test registry
    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }

    // add suites
    if(CUE_SUCCESS != addSuite("MainSuite", MainSuite_init, MainSuite_clean, MainSuite_tests)) {
        return CU_get_error();
    }
    if(CUE_SUCCESS != addSuite("DBSuite", DBSuite_init, DBSuite_clean, DBSuite_tests)) {
        return CU_get_error();
    }
    if(CUE_SUCCESS != addSuite("SQLSuite", SQLSuite_init, SQLSuite_clean, SQLSuite_tests)) {
        return CU_get_error();
    }
    if(CUE_SUCCESS != addSuite("MemorySuite", MemorySuite_init, MemorySuite_clean, MemorySuite_tests)) {
        return CU_get_error();
    }
    if(CUE_SUCCESS != addSuite("SQLFuzzSuite", SQLFuzzSuite_init, SQLFuzzSuite_clean, SQLFuzzSuite_tests)) {
        return CU_get_error();
    }
    if(argc > 1 && !strcmp(argv[1], "bench")) {
        if(CUE_SUCCESS != addSuite("BenchSuite", BenchSuite_init, BenchSuite_clean, BenchSuite_tests)) {
            return CU_get_error();
        }
    }
    
    // Run all tests using the CUnit Basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    
    //CU_cleanup_registry();
    int exitCode = CU_get_error();
    EagleLogger_Delete(EagleLogger_Get());
    
    // may need to wait
    if(wait) {
        sleep(3600);
    }
    
    return exitCode;
}
