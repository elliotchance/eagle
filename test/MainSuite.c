#include <stdio.h>
#include <stdlib.h>
#include "MainSuite.h"
#include "EaglePage.h"
#include "EagleData.h"
#include "EaglePageOperations.h"

/**
 * The suite init function.
 */
int MainSuite_init()
{
    return 0;
}

/**
 * The suite cleanup function.
 */
int MainSuite_clean()
{
    return 0;
}

CUnitTests* MainSuite_tests()
{
    CUnitTests *tests = CUnitTests_New(10);
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, testPageGreaterThanInt));
    return tests;
}

CUNIT_TEST(MainSuite, testPageGreaterThanInt)
{
    int pageSize = 1000;
    int testValue = rand();
    EaglePage *page = EaglePage_Alloc(pageSize);
    EaglePage *out = EaglePage_Alloc(pageSize);
    
    // prepare
    for(int i = 0; i < pageSize; ++i) {
        page->data[i] = i;
    }
    
    EaglePageOperations_GreaterThanInt(page, out, EagleData_Int(testValue));
    
    // verify
    int valid = 1;
    for(int i = 0; i < pageSize; ++i) {
        if(!(page->data[i] > testValue == out->data[i])) {
            valid = 0;
            break;
        }
    }
    CU_ASSERT(valid);
}
