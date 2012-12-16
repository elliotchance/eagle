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
    
    int *int1 = EagleData_Int(testValue);
    EaglePageOperations_GreaterThanInt(page, out, int1);
    
    // verify
    int valid = 1;
    for(int i = 0; i < pageSize; ++i) {
        if(!(page->data[i] > testValue == out->data[i])) {
            valid = 0;
            break;
        }
    }
    CUNIT_ASSERT_EQUAL_INT(valid, 1);
    
    // clean up
    free(int1);
    EaglePage_Delete(page);
    EaglePage_Delete(out);
}

CUNIT_TEST(MainSuite, EaglePageOperations_LessThanInt)
{
    int pageSize = 1000;
    int testValue = rand();
    EaglePage *page = MainSuite_GeneratePage(pageSize);
    EaglePage *out = EaglePage_Alloc(pageSize);
    
    int *int1 = EagleData_Int(testValue);
    EaglePageOperations_LessThanInt(page, out, int1);
    
    // verify
    int valid = 1;
    for(int i = 0; i < pageSize; ++i) {
        if(!(page->data[i] < testValue == out->data[i])) {
            valid = 0;
            break;
        }
    }
    CUNIT_ASSERT_EQUAL_INT(valid, 1);
    
    // clean up
    free(int1);
    EaglePage_Delete(page);
    EaglePage_Delete(out);
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
    CUNIT_ASSERT_EQUAL_INT(valid, 1);
    
    // clean up
    EaglePage_Delete(page1);
    EaglePage_Delete(page2);
    EaglePage_Delete(out);
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
    
    EaglePageProvider *provider = EaglePageProvider_CreateFromIntArray(data, totalRecords, recordsPerPage);
    CUNIT_ASSERT_EQUAL_INT(EaglePageProvider_pagesRemaining(provider), EaglePageProvider_TotalPages(totalRecords, recordsPerPage));
    
    EaglePlan_addBufferProvider(plan, EaglePlanBufferProvider_New(1, provider));
    
    EaglePlan_addOperation(plan, EaglePlanOperation_New(2, EaglePageOperations_GreaterThanInt, 1, EagleData_Int(min), EagleTrue, "1"));
    EaglePlan_addOperation(plan, EaglePlanOperation_New(3, EaglePageOperations_LessThanInt,    1, EagleData_Int(max), EagleTrue, "2"));
    EaglePlan_addOperation(plan, EaglePlanOperation_NewPage(0, EaglePageOperations_AndPage,    2, 3,                  "3"));
    
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
    CUNIT_VERIFY_EQUAL_INT(matches, receiver->used);
    
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
    CUNIT_VERIFY_EQUAL_INT(matches, receiver->used);
    CUNIT_VERIFY_EQUAL_INT(misses, (totalRecords - receiver->used));
    
    // clean up
    EagleInstance_Delete(eagle);
}

CUNIT_TEST(MainSuite, _, InstanceSingle)
{
    _instanceTest(1, 1, 100);
    _instanceTest(1, 10, 1000);
    _instanceTest(1, 100, 1000);
    _instanceTest(1, 1000, 1000);
    _instanceTest(1, 1000, 1001);
    _instanceTest(1, 1000, 10000);
}

CUNIT_TEST(MainSuite, _, InstanceMulti)
{
    _instanceTest(8, 1, 100);
    _instanceTest(8, 10, 1000);
    _instanceTest(8, 100, 1000);
    _instanceTest(8, 1000, 1000);
    _instanceTest(8, 1000, 1001);
    _instanceTest(8, 1000, 10000);
}

CUNIT_TEST(MainSuite, EaglePageProvider_TotalPages)
{
    CUNIT_VERIFY_EQUAL_INT(EaglePageProvider_TotalPages(1000, 1000), 1);
    CUNIT_VERIFY_EQUAL_INT(EaglePageProvider_TotalPages(1001, 1000), 2);
    CUNIT_VERIFY_EQUAL_INT(EaglePageProvider_TotalPages(1000, 10), 100);
}

CUNIT_TEST(MainSuite, EaglePageProvider_CreateFromIntStream)
{
    int testDataSize = 3, recordsPerPage = 2;
    int *testData = (int*) calloc(testDataSize, sizeof(int));
    testData[0] = 123;
    testData[1] = 456;
    testData[2] = 789;
    
    EaglePageProvider *provider = EaglePageProvider_CreateFromIntArray(testData, testDataSize, recordsPerPage);
    CUNIT_VERIFY_EQUAL_INT(provider->totalRecords, testDataSize);
    CUNIT_VERIFY_EQUAL_INT(provider->recordsPerPage, recordsPerPage);
    
    CUNIT_VERIFY_EQUAL_INT(EaglePageProvider_pagesRemaining(provider), 2);
    EaglePage *page1 = EaglePageProvider_nextPage(provider);
    CUNIT_VERIFY_EQUAL_INT(page1->count, recordsPerPage);
    CUNIT_VERIFY_EQUAL_INT(page1->data[0], testData[0]);
    CUNIT_VERIFY_EQUAL_INT(page1->data[1], testData[1]);
    CUNIT_VERIFY_EQUAL_INT(page1->recordOffset, 0);
    
    CUNIT_VERIFY_EQUAL_INT(EaglePageProvider_pagesRemaining(provider), 1);
    EaglePage *page2 = EaglePageProvider_nextPage(provider);
    CUNIT_VERIFY_EQUAL_INT(page2->count, 1);
    CUNIT_VERIFY_EQUAL_INT(page2->data[0], testData[2]);
    CUNIT_VERIFY_EQUAL_INT(page2->recordOffset, recordsPerPage);
    
    CUNIT_VERIFY_EQUAL_INT(EaglePageProvider_pagesRemaining(provider), 0);
    
    // clean up
    EaglePageProvider_Delete(provider);
    EaglePage_Delete(page1);
    EaglePage_Delete(page2);
}

CUNIT_TEST(MainSuite, EaglePlanOperation_toString)
{
    EaglePlanOperation *op = EaglePlanOperation_New(0, NULL, 0, NULL, EagleFalse, "some description");
    char *msg = (char*) EaglePlanOperation_toString(op);
    CUNIT_ASSERT_EQUAL_STRING(msg, "some description");
    EaglePlanOperation_Delete(op);
}

CUNIT_TEST(MainSuite, EaglePlan_Delete)
{
    EaglePlan_Delete(NULL);
}

CUNIT_TEST(MainSuite, EaglePlan_toString)
{
    EaglePlan *plan = EaglePlan_New(0, NULL);
    char *msg = (char*) EaglePlan_toString(plan);
    CUNIT_ASSERT_EQUAL_STRING(msg, "EaglePlan:\n");
    free(msg);
    
    // add some buffer providers
    EaglePageProvider *provider = EaglePageProvider_CreateFromIntArray(NULL, 0, 10);
    EaglePlanBufferProvider *bp = EaglePlanBufferProvider_New(123, provider);
    EaglePlan_addBufferProvider(plan, bp);
    
    // add some steps
    EaglePlan_addOperation(plan, EaglePlanOperation_New(2, EaglePageOperations_GreaterThanInt, 1, NULL, EagleFalse, "Step 1"));
    EaglePlan_addOperation(plan, EaglePlanOperation_New(3, EaglePageOperations_LessThanInt,    1, NULL, EagleFalse, "Step 2"));
    EaglePlan_addOperation(plan, EaglePlanOperation_NewPage(0, EaglePageOperations_AndPage,    2, 3,    "Step 3"));
    
    msg = (char*) EaglePlan_toString(plan);
    CUNIT_ASSERT_EQUAL_STRING(msg, "EaglePlan:\n  destination = 123\n  Step 1\n  Step 2\n  Step 3\n");
    free(msg);
    
    EaglePlan_Delete(plan);
}

CUNIT_TEST(MainSuite, EaglePageReceiver_pushRecordId)
{
    EaglePageReceiver *receiver = EaglePageReceiver_New();
    
    // fill
    for(int i = 0; i < receiver->allocated; ++i) {
        EaglePageReceiver_pushRecordId(receiver, 0);
    }
    CUNIT_ASSERT_EQUAL_INT(receiver->allocated, receiver->used);
    
    // try to push in more data than is allocated
    EaglePageReceiver_pushRecordId(receiver, 0);
    CUNIT_ASSERT_EQUAL_INT(receiver->allocated, receiver->used);
    
    EaglePageReceiver_Delete(receiver);
}

CUNIT_TEST(MainSuite, EaglePlanBufferProvider_toString)
{
    EaglePageProvider *provider = EaglePageProvider_CreateFromIntArray(NULL, 0, 10);
    EaglePlanBufferProvider *bp = EaglePlanBufferProvider_New(123, provider);
    char *description = EaglePlanBufferProvider_toString(bp);
    CUNIT_ASSERT_EQUAL_STRING(description, "destination = 123");
    free(description);
    EaglePlanBufferProvider_Delete(bp);
}

CUNIT_TEST(MainSuite, EaglePlanJob_Delete)
{
    EaglePlanJob_Delete(NULL);
}

CUNIT_TEST(MainSuite, EaglePageProvider_Delete)
{
    EaglePageProvider_Delete(NULL);
}

CUNIT_TEST(MainSuite, EaglePageProvider_add)
{
    EaglePageProvider *provider = EaglePageProvider_CreateFromIntArray(NULL, 0, 1);
    CUNIT_ASSERT_EQUAL_INT(EaglePageProvider_add(provider, NULL), EagleFalse);
    EaglePageProvider_Delete(provider);
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
    CUnitTests *tests = CUnitTests_New(100);
    
    // method tests
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePageOperations_GreaterThanInt));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePageOperations_LessThanInt));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePageOperations_AndPage));
    
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePageProvider_TotalPages));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePageProvider_CreateFromIntStream));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePageProvider_Delete));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePageProvider_add));
    
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePageReceiver_pushRecordId));
    
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePlan_Delete));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePlan_toString));
    
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePlanBufferProvider_toString));
    
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePlanJob_Delete));
    
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePlanOperation_toString));
    
    // complex / execution tests
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, _, InstanceSingle));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, _, InstanceMulti));
    
    return tests;
}
