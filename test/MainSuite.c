#include <stdio.h>
#include <stdlib.h>
#include "MainSuite.h"
#include "EagleData.h"
#include "EaglePageOperations.h"
#include "EagleInstance.h"
#include "EaglePageReceiver.h"

EaglePage* MainSuite_GeneratePage(int pageSize)
{
    EaglePage *page = EaglePage_Alloc(pageSize);
    
    // prepare
    for(int i = 0; i < pageSize; ++i) {
        page->data[i] = i;
    }
    
    return page;
}

CUNIT_TEST(MainSuite, EaglePageOperations_GreaterThanInt)
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

CUNIT_TEST(MainSuite, EaglePageOperations_LessThanInt)
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

CUNIT_TEST(MainSuite, EaglePageOperations_AndPage)
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

void _instanceTest(int cores, int recordsPerPage, int totalRecords)
{
    // initialise workers
    EagleInstance *eagle = EagleInstance_New(cores);
    
    // prepare data
    int *data = (int*) calloc(sizeof(int), totalRecords);
    for(int i = 0; i < totalRecords; ++i) {
        data[i] = rand();
    }
    
    // plan: ? BETWEEN ? AND 20000000
    int min = 10000000, max = 20000000;
    EaglePageReceiver *receiver = EaglePageReceiver_New();
    EaglePlan *plan = EaglePlan_New(recordsPerPage, receiver);
    EaglePageProvider *provider = EaglePageProvider_CreateFromIntStream(data, totalRecords, recordsPerPage);
    EaglePlan_addBufferProvider(plan, EaglePlanBufferProvider_New(1, provider));
    
    EaglePlan_addOperation(plan, EaglePlanOperation_New(2, EaglePageOperations_GreaterThanInt, 1, EagleData_Int(min), ""));
    EaglePlan_addOperation(plan, EaglePlanOperation_New(3, EaglePageOperations_LessThanInt,    1, EagleData_Int(max), ""));
    EaglePlan_addOperation(plan, EaglePlanOperation_NewPage(0, EaglePageOperations_AndPage,    2, 3,                  ""));
    
    EagleInstance_addPlan(eagle, plan);
    
    // run
    EagleInstance_run(eagle);
    
    // make sure all the values back match the expression
    int matches = 0;
    for(int i = 0; i < receiver->used; ++i) {
        int value = data[receiver->buffer[i]];
        if(value > min && value < max) {
            ++matches;
        }
    }
    CU_ASSERT(matches == receiver->used);
    
    int misses = 0;
    matches = 0;
    for(int i = 0; i < totalRecords; ++i) {
        if(data[i] > min && data[i] < max) {
            ++matches;
        }
        else {
            ++misses;
        }
    }
    CU_ASSERT(matches == receiver->used);
    CU_ASSERT(misses == (totalRecords - receiver->used));
}

CUNIT_TEST(MainSuite, _, InstanceSingle)
{
    _instanceTest(1, 1, 100);
    _instanceTest(1, 10, 1000);
    _instanceTest(1, 100, 1000);
    _instanceTest(1, 1000, 1000);
    _instanceTest(1, 1001, 1000);
    _instanceTest(1, 1000, 10000);
}

CUNIT_TEST(MainSuite, _, InstanceMulti)
{
    _instanceTest(8, 1, 100);
    _instanceTest(8, 10, 1000);
    _instanceTest(8, 100, 1000);
    _instanceTest(8, 1000, 1000);
    _instanceTest(8, 1001, 1000);
    _instanceTest(8, 1000, 10000);
}

CUNIT_TEST(MainSuite, EaglePageProvider_TotalPages)
{
    CU_ASSERT_EQUAL(EaglePageProvider_TotalPages(1000, 1000), 1);
    CU_ASSERT_EQUAL(EaglePageProvider_TotalPages(1001, 1000), 2);
    CU_ASSERT_EQUAL(EaglePageProvider_TotalPages(1000, 10), 100);
}

CUNIT_TEST(MainSuite, EaglePageProvider_CreateFromIntStream)
{
    int testDataSize = 3, recordsPerPage = 2;
    int *testData = (int*) calloc(testDataSize, sizeof(int));
    testData[0] = 123;
    testData[1] = 456;
    testData[2] = 789;
    
    EaglePageProvider *provider = EaglePageProvider_CreateFromIntStream(testData, testDataSize, recordsPerPage);
    CU_ASSERT_EQUAL(provider->totalRecords, testDataSize);
    CU_ASSERT_EQUAL(provider->recordsPerPage, recordsPerPage);
    
    EaglePage *page1 = EaglePageProvider_nextPage(provider);
    CU_ASSERT_EQUAL(page1->count, recordsPerPage);
    CU_ASSERT_EQUAL(page1->data[0], testData[0]);
    CU_ASSERT_EQUAL(page1->data[1], testData[1]);
    CU_ASSERT_EQUAL(page1->recordOffset, 0);
    
    EaglePage *page2 = EaglePageProvider_nextPage(provider);
    CU_ASSERT_EQUAL(page2->count, 1);
    CU_ASSERT_EQUAL(page2->data[0], testData[2]);
    CU_ASSERT_EQUAL(page2->recordOffset, recordsPerPage);
    
    // clean up
    free(testData);
}

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
    
    // method tests
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePageOperations_GreaterThanInt));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePageOperations_LessThanInt));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePageOperations_AndPage));
    
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePageProvider_TotalPages));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePageProvider_CreateFromIntStream));
    
    // complex / execution tests
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, _, InstanceSingle));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, _, InstanceMulti));
    
    return tests;
}
