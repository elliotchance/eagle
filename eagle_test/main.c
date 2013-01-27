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

    {
        // add a suite to the registry
        CU_pSuite pSuite = CU_add_suite("MainSuite", MainSuite_init, MainSuite_clean);
        if(NULL == pSuite) {
            CU_cleanup_registry();
            return CU_get_error();
        }
        
        // add the tests to the suite
        CUnitTests *tests = MainSuite_tests();
        for(int i = 0; i < tests->usedTests; ++i) {
            CUnitTest *test = tests->tests[i];
            if(NULL == CU_add_test(pSuite, test->strName, test->pTestFunc)) {
                CU_cleanup_registry();
                return CU_get_error();
            }
        }
        
        // clean up
        CUnitTests_Delete(tests);
    }
    
    {
        // add a suite to the registry
        CU_pSuite pSuite = CU_add_suite("DBSuite", DBSuite_init, DBSuite_clean);
        if(NULL == pSuite) {
            CU_cleanup_registry();
            return CU_get_error();
        }
        
        // add the tests to the suite
        CUnitTests *tests = DBSuite_tests();
        for(int i = 0; i < tests->usedTests; ++i) {
            CUnitTest *test = tests->tests[i];
            if(NULL == CU_add_test(pSuite, test->strName, test->pTestFunc)) {
                CU_cleanup_registry();
                return CU_get_error();
            }
        }
        
        // clean up
        CUnitTests_Delete(tests);
    }
    
    /*{
        // add a suite to the registry
        CU_pSuite pSuite = CU_add_suite("SQLSuite", SQLSuite_init, SQLSuite_clean);
        if(NULL == pSuite) {
            CU_cleanup_registry();
            return CU_get_error();
        }
        
        // add the tests to the suite
        CUnitTests *tests = SQLSuite_tests();
        for(int i = 0; i < tests->usedTests; ++i) {
            CUnitTest *test = tests->tests[i];
            if(NULL == CU_add_test(pSuite, test->strName, test->pTestFunc)) {
                CU_cleanup_registry();
                return CU_get_error();
            }
        }
        
        // clean up
        CUnitTests_Delete(tests);
    }
    
    {
        // add a suite to the registry
        CU_pSuite pSuite = CU_add_suite("MemorySuite", MemorySuite_init, MemorySuite_clean);
        if(NULL == pSuite) {
            CU_cleanup_registry();
            return CU_get_error();
        }
        
        // add the tests to the suite
        CUnitTests *tests = MemorySuite_tests();
        for(int i = 0; i < tests->usedTests; ++i) {
            CUnitTest *test = tests->tests[i];
            if(NULL == CU_add_test(pSuite, test->strName, test->pTestFunc)) {
                CU_cleanup_registry();
                return CU_get_error();
            }
        }
        
        // clean up
        CUnitTests_Delete(tests);
    }*/
    
    if(argc > 1 && !strcmp(argv[1], "bench"))
    {
        // add a suite to the registry
        CU_pSuite pSuite = CU_add_suite("BenchSuite", BenchSuite_init, BenchSuite_clean);
        if(NULL == pSuite) {
            CU_cleanup_registry();
            return CU_get_error();
        }
        
        // add the tests to the suite
        CUnitTests *tests = BenchSuite_tests();
        for(int i = 0; i < tests->usedTests; ++i) {
            CUnitTest *test = tests->tests[i];
            if(NULL == CU_add_test(pSuite, test->strName, test->pTestFunc)) {
                CU_cleanup_registry();
                return CU_get_error();
            }
        }
        
        // clean up
        CUnitTests_Delete(tests);
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
