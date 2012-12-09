#include <stdio.h>
#include <stdlib.h>
#include "MainSuite.h"
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
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, testPageLessThanInt));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, testPageAnd));
    return tests;
}

EaglePage* MainSuite_GeneratePage(int pageSize)
{
    EaglePage *page = EaglePage_Alloc(pageSize);
    
    // prepare
    for(int i = 0; i < pageSize; ++i) {
        page->data[i] = i;
    }
    
    return page;
}

CUNIT_TEST(MainSuite, testPageGreaterThanInt)
{
    int pageSize = 1000;
    int testValue = rand();
    EaglePage *page = MainSuite_GeneratePage(pageSize);
    EaglePage *out = EaglePage_Alloc(pageSize);
    
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

CUNIT_TEST(MainSuite, testPageLessThanInt)
{
    int pageSize = 1000;
    int testValue = rand();
    EaglePage *page = MainSuite_GeneratePage(pageSize);
    EaglePage *out = EaglePage_Alloc(pageSize);
    
    EaglePageOperations_LessThanInt(page, out, EagleData_Int(testValue));
    
    // verify
    int valid = 1;
    for(int i = 0; i < pageSize; ++i) {
        if(!(page->data[i] < testValue == out->data[i])) {
            valid = 0;
            break;
        }
    }
    CU_ASSERT(valid);
}

CUNIT_TEST(MainSuite, testPageAnd)
{
    int pageSize = 1000;
    EaglePage *page1 = EaglePage_Alloc(pageSize);
    EaglePage *page2 = EaglePage_Alloc(pageSize);
    EaglePage *out = EaglePage_Alloc(pageSize);
    
    for(int i = 0; i < pageSize; ++i) {
        page1->data[i] = rand() % 2;
        page2->data[i] = rand() % 2;
    }
    
    EaglePageOperations_AndPage(page1, out, (void*) page2);
    
    // verify
    int valid = 1;
    for(int i = 0; i < pageSize; ++i) {
        if(page1->data[i] && page2->data[i] != out->data[i]) {
            valid = 0;
            break;
        }
    }
    CU_ASSERT(valid);
}
