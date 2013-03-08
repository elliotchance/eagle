#include "TestSuite.h"
#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "MainSuite.h"
#include "DBSuite.h"
#include "SQLSuite.h"
#include "MemorySuite.h"
#include "BenchSuite.h"
#include "EagleLogger.h"
#include "SQLFuzzSuite.h"
#include "OperatorSuite.h"

CU_ErrorCode addSuiteSingle(const char *name, CU_InitializeFunc pInit, CU_CleanupFunc pClean, CUnitTests* (*testsFunction)())
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

CU_ErrorCode addSuite(const char *name, CU_InitializeFunc pInit, CU_CleanupFunc pClean, CU_pSuite *pSuite)
{
    // add a suite to the registry
    *pSuite = CU_add_suite(name, pInit, pClean);
    if(NULL == *pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    return CUE_SUCCESS;
}

CU_ErrorCode addSuiteTests(CU_pSuite pSuite, CUnitTests* (*testsFunction)())
{
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
    // usage
    if(argc < 2) {
        fprintf(stderr, "\n");
        fprintf(stderr, "Usage: eagle_test [--wait] [--all-suites] [--suite-X] [--exclude-X]\n");
        fprintf(stderr, "         --wait        Keep process alive for 1 hour after the tests finish.\n");
        fprintf(stderr, "         --all-suites  Run all test suites.\n");
        fprintf(stderr, "         --suite-X     X can be:\n");
        fprintf(stderr, "         --exclude-X     main\n");
        fprintf(stderr, "                         db\n");
        fprintf(stderr, "                         sql\n");
        fprintf(stderr, "                         memory\n");
        fprintf(stderr, "                         sqlfuzz\n");
        fprintf(stderr, "                         bench\n");
        fprintf(stderr, "                         operator\n");
        fprintf(stderr, "\n");
        exit(1);
    }
    
    // options
    EagleBoolean op_wait = EagleFalse;
    EagleBoolean op_suite_main = EagleFalse;
    EagleBoolean op_suite_db = EagleFalse;
    EagleBoolean op_suite_sql = EagleFalse;
    EagleBoolean op_suite_memory = EagleFalse;
    EagleBoolean op_suite_sqlfuzz = EagleFalse;
    EagleBoolean op_suite_bench = EagleFalse;
    EagleBoolean op_suite_operator = EagleFalse;
    
    for(int i = 1; i < argc; ++i) {
        if(!strcmp(argv[i], "--wait")) {
            op_wait = EagleTrue;
        }
        else if(!strcmp(argv[i], "--suite-main")) {
            op_suite_main = EagleTrue;
        }
        else if(!strcmp(argv[i], "--suite-db")) {
            op_suite_db = EagleTrue;
        }
        else if(!strcmp(argv[i], "--suite-sql")) {
            op_suite_sql = EagleTrue;
        }
        else if(!strcmp(argv[i], "--suite-memory")) {
            op_suite_memory = EagleTrue;
        }
        else if(!strcmp(argv[i], "--suite-sqlfuzz")) {
            op_suite_sqlfuzz = EagleTrue;
        }
        else if(!strcmp(argv[i], "--suite-bench")) {
            op_suite_bench = EagleTrue;
        }
        else if(!strcmp(argv[i], "--suite-operator")) {
            op_suite_operator = EagleTrue;
        }
        else if(!strcmp(argv[i], "--all-suites")) {
            op_suite_main = EagleTrue;
            op_suite_db = EagleTrue;
            op_suite_sql = EagleTrue;
            op_suite_memory = EagleTrue;
            op_suite_sqlfuzz = EagleTrue;
            op_suite_bench = EagleTrue;
            op_suite_operator = EagleTrue;
        }
        else if(!strcmp(argv[i], "--exclude-main")) {
            op_suite_main = EagleFalse;
        }
        else if(!strcmp(argv[i], "--exclude-db")) {
            op_suite_db = EagleFalse;
        }
        else if(!strcmp(argv[i], "--exclude-sql")) {
            op_suite_sql = EagleFalse;
        }
        else if(!strcmp(argv[i], "--exclude-memory")) {
            op_suite_memory = EagleFalse;
        }
        else if(!strcmp(argv[i], "--exclude-sqlfuzz")) {
            op_suite_sqlfuzz = EagleFalse;
        }
        else if(!strcmp(argv[i], "--exclude-bench")) {
            op_suite_bench = EagleFalse;
        }
        else if(!strcmp(argv[i], "--exclude-operator")) {
            op_suite_operator = EagleFalse;
        }
        else {
            fprintf(stderr, "\nUnknown option \"%s\"\n\n", argv[i]);
            exit(1);
        }
    }
    
    // initialize the CUnit test registry
    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }

    // add suites
    CU_pSuite suite;
    if(EagleTrue == op_suite_main) {
        if(CUE_SUCCESS != addSuite("MainSuite", MainSuite_init, MainSuite_clean, &suite) ||
           CUE_SUCCESS != addSuiteTests(suite, MainSuite1_tests) ||
           CUE_SUCCESS != addSuiteTests(suite, MainSuite2_tests)) {
               return CU_get_error();
        }
    }
    
    if(EagleTrue == op_suite_db) {
        if(CUE_SUCCESS != addSuite("DBSuite", DBSuite_init, DBSuite_clean, &suite) ||
           CUE_SUCCESS != addSuiteTests(suite, DBSuite1_tests) ||
           CUE_SUCCESS != addSuiteTests(suite, DBSuite2_tests)) {
            return CU_get_error();
        }
    }
    
    if(EagleTrue == op_suite_sql && CUE_SUCCESS != addSuiteSingle("SQLSuite", SQLSuite_init, SQLSuite_clean, SQLSuite_tests)) {
        return CU_get_error();
    }
    
    if(EagleTrue == op_suite_memory && CUE_SUCCESS != addSuiteSingle("MemorySuite", MemorySuite_init, MemorySuite_clean, MemorySuite_tests)) {
        return CU_get_error();
    }
    
    if(EagleTrue == op_suite_sqlfuzz && CUE_SUCCESS != addSuiteSingle("SQLFuzzSuite", SQLFuzzSuite_init, SQLFuzzSuite_clean, SQLFuzzSuite_tests)) {
        return CU_get_error();
    }
    
    if(EagleTrue == op_suite_bench && CUE_SUCCESS != addSuiteSingle("BenchSuite", BenchSuite_init, BenchSuite_clean, BenchSuite_tests)) {
        return CU_get_error();
    }
    
    if(EagleTrue == op_suite_operator && CUE_SUCCESS != addSuiteSingle("OperatorSuite", OperatorSuite_init, OperatorSuite_clean, OperatorSuite_tests)) {
        return CU_get_error();
    }
    
    // Run all tests using the CUnit Basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    
    //CU_cleanup_registry();
    int exitCode = CU_get_error();
    EagleLogger_Delete(EagleLogger_Get());
    
    // may need to wait
    if(EagleTrue == op_wait) {
        sleep(3600);
    }
    
    return exitCode;
}
