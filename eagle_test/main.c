#include <CUnit/Basic.h>
#include "TestSuite.h"
#include "MainSuite.h"
#include "DBSuite.h"

/**
 * The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another CUnit error code on failure.
 */
int main(int argc, char **argv)
{
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
    }
    
    // Run all tests using the CUnit Basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
