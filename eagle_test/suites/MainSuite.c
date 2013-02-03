#include "MainSuite.h"
#include <stdio.h>
#include <stdlib.h>
#include "EagleData.h"
#include "EaglePageOperations.h"
#include "EagleInstance.h"
#include "EagleLinkedList.h"
#include "EagleMemory.h"
#include "EagleLogger.h"

EaglePage* MainSuite_GeneratePage(int pageSize)
{
    EaglePage *page = EaglePage_AllocInt(pageSize);
    
    // prepare
    for(int i = 0; i < pageSize; ++i) {
        ((int*) page->data)[i] = i;
    }
    
    return page;
}

CUNIT_TEST(MainSuite, EaglePageOperations_GreaterThanInt)
{
    int pageSize = 1000;
    int testValue = arc4random();
    EaglePage *page = MainSuite_GeneratePage(pageSize);
    EaglePage *out = EaglePage_AllocInt(pageSize);
    
    int *int1 = EagleData_Int(testValue);
    EaglePageOperations_GreaterThanInt(out, page, NULL, int1);
    
    // verify
    int valid = 1;
    for(int i = 0; i < pageSize; ++i) {
        if(!(((int*) page->data)[i] > testValue == ((int*) out->data)[i])) {
            valid = 0;
            break;
        }
    }
    CUNIT_ASSERT_EQUAL_INT(valid, 1);
    
    // clean up
    EagleMemory_Free(int1);
    EaglePage_Delete(page);
    EaglePage_Delete(out);
}

CUNIT_TEST(MainSuite, EaglePageOperations_LessThanInt)
{
    int pageSize = 1000;
    int testValue = arc4random();
    EaglePage *page = MainSuite_GeneratePage(pageSize);
    EaglePage *out = EaglePage_AllocInt(pageSize);
    
    int *int1 = EagleData_Int(testValue);
    EaglePageOperations_LessThanInt(out, page, NULL, int1);
    
    // verify
    int valid = 1;
    for(int i = 0; i < pageSize; ++i) {
        if(!(((int*) page->data)[i] < testValue == ((int*) out->data)[i])) {
            valid = 0;
            break;
        }
    }
    CUNIT_ASSERT_EQUAL_INT(valid, 1);
    
    // clean up
    EagleMemory_Free(int1);
    EaglePage_Delete(page);
    EaglePage_Delete(out);
}

CUNIT_TEST(MainSuite, EaglePageOperations_AndPage)
{
    int pageSize = 1000;
    EaglePage *page1 = EaglePage_AllocInt(pageSize);
    EaglePage *page2 = EaglePage_AllocInt(pageSize);
    EaglePage *out = EaglePage_AllocInt(pageSize);
    
    for(int i = 0; i < pageSize; ++i) {
        ((int*) page1->data)[i] = arc4random() % 2;
        ((int*) page2->data)[i] = arc4random() % 2;
    }
    
    EaglePageOperations_AndPage(out, page1, page2, NULL);
    
    // verify
    int valid = 1;
    for(int i = 0; i < pageSize; ++i) {
        if(((int*) page1->data)[i] && ((int*) page2->data)[i] != ((int*) out->data)[i]) {
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
        data[i] = arc4random();
    }
    
    // plan: ? BETWEEN ? AND 20000000
    int min = 10000000, max = 20000000;
    EaglePlan *plan = EaglePlan_New(recordsPerPage);
    
    // input data
    EaglePageProvider *provider = EaglePageProvider_CreateFromIntArray(data, totalRecords, recordsPerPage, NULL);
    CUNIT_ASSERT_EQUAL_INT(EaglePageProvider_pagesRemaining(provider), EaglePageProvider_TotalPages(totalRecords, recordsPerPage));
    EaglePlanBufferProvider *bp = EaglePlanBufferProvider_New(1, provider, EagleTrue);
    EaglePlan_addBufferProvider(plan, bp, EagleTrue);
    
    // destination provider
    // We are being a bit naughty here to use totalRecords instead of recordsPerPage so that we can guarantee all the
    // results will fall into the first page.
    EaglePageProvider *result = EaglePageProvider_CreateFromStream(EagleDataTypeInteger, totalRecords, "answer");
    
    // plan
    EaglePlanOperation *op1, *op2, *op3, *op4;
    EaglePlan_addOperation(plan, op1 = EaglePlanOperation_New(EaglePageOperations_GreaterThanInt,      2, 1, -1, EagleData_Int(min), EagleTrue,  "1"));
    EaglePlan_addOperation(plan, op2 = EaglePlanOperation_New(EaglePageOperations_LessThanInt,         3, 1, -1, EagleData_Int(max), EagleTrue,  "2"));
    EaglePlan_addOperation(plan, op3 = EaglePlanOperation_New(EaglePageOperations_AndPage,             0, 2,  3, NULL,               EagleFalse, "3"));
    EaglePlan_addOperation(plan, op4 = EaglePlanOperation_New(EaglePageOperations_SendPageToProvider, -1, 0,  1, result,             EagleFalse, "4"));
    
    // this will be enough buffers for the above operations
    plan->buffersNeeded = 10;
    plan->bufferTypes = (EagleDataType*) calloc(plan->buffersNeeded, sizeof(EagleDataType));
    for(int i = 0; i < plan->buffersNeeded; ++i) {
        plan->bufferTypes[i] = EagleDataTypeInteger;
    }
    
    EagleInstance_addPlan(eagle, plan);
    
    // run
    EagleInstance_run(eagle);
    
    // make sure all the values back match to the expression, there must be one and only page
    EaglePage *resultPage = EaglePageProvider_nextPage(result);
    CUNIT_VERIFY_NULL(EaglePageProvider_nextPage(result));
    
    int matches = 0;
    if(NULL != resultPage) {
        for(int i = 0; i < resultPage->count; ++i) {
            int value = ((int*) resultPage->data)[i];
            if(value > min && value < max) {
                ++matches;
            }
        }
        CUNIT_VERIFY_EQUAL_INT(matches, resultPage->count);
    }
    
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
    CUNIT_VERIFY_EQUAL_INT(matches, (NULL == resultPage ? 0 : resultPage->count));
    CUNIT_VERIFY_EQUAL_INT(misses, (totalRecords - (NULL == resultPage ? 0 : resultPage->count)));
    
    // clean up
    free(data);
    EaglePlanOperation_Delete(op1);
    EaglePlanOperation_Delete(op2);
    EaglePlanOperation_Delete(op3);
    EaglePlanOperation_Delete(op4);
    EaglePage_Delete(resultPage);
    EaglePageProvider_Delete(result);
    EaglePlan_Delete(plan);
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

CUNIT_TEST(MainSuite, EaglePageProvider_CreateFromIntArray)
{
    int testDataSize = 3, recordsPerPage = 2;
    int *testData = (int*) calloc(testDataSize, sizeof(int));
    testData[0] = 123;
    testData[1] = 456;
    testData[2] = 789;
    
    EaglePageProvider *provider = EaglePageProvider_CreateFromIntArray(testData, testDataSize, recordsPerPage, NULL);
    CUNIT_VERIFY_EQUAL_INT(provider->totalRecords, testDataSize);
    CUNIT_VERIFY_EQUAL_INT(provider->recordsPerPage, recordsPerPage);
    
    // read first page
    CUNIT_VERIFY_EQUAL_INT(EaglePageProvider_pagesRemaining(provider), 2);
    EaglePage *page1 = EaglePageProvider_nextPage(provider);
    CUNIT_VERIFY_EQUAL_INT(page1->count, recordsPerPage);
    CUNIT_VERIFY_EQUAL_INT(((int*) page1->data)[0], testData[0]);
    CUNIT_VERIFY_EQUAL_INT(((int*) page1->data)[1], testData[1]);
    CUNIT_VERIFY_EQUAL_INT(page1->recordOffset, 0);
    EaglePage_Delete(page1);
    
    // reset
    EaglePageProvider_reset(provider);
    
    // reread the first page and beyond
    CUNIT_VERIFY_EQUAL_INT(EaglePageProvider_pagesRemaining(provider), 2);
    page1 = EaglePageProvider_nextPage(provider);
    CUNIT_VERIFY_EQUAL_INT(page1->count, recordsPerPage);
    CUNIT_VERIFY_EQUAL_INT(((int*) page1->data)[0], testData[0]);
    CUNIT_VERIFY_EQUAL_INT(((int*) page1->data)[1], testData[1]);
    CUNIT_VERIFY_EQUAL_INT(page1->recordOffset, 0);
    EaglePage_Delete(page1);
    
    CUNIT_VERIFY_EQUAL_INT(EaglePageProvider_pagesRemaining(provider), 1);
    EaglePage *page2 = EaglePageProvider_nextPage(provider);
    CUNIT_VERIFY_EQUAL_INT(page2->count, 1);
    CUNIT_VERIFY_EQUAL_INT(((int*) page2->data)[0], testData[2]);
    CUNIT_VERIFY_EQUAL_INT(page2->recordOffset, recordsPerPage);
    EaglePage_Delete(page2);
    
    CUNIT_VERIFY_EQUAL_INT(EaglePageProvider_pagesRemaining(provider), 0);
    
    // clean up
    free(testData);
    EaglePageProvider_Delete(provider);
}

CUNIT_TEST(MainSuite, EaglePlanOperation_toString)
{
    EaglePlanOperation *op = EaglePlanOperation_New(NULL, -1, -1, -1, NULL, EagleFalse, "some description");
    char *msg = (char*) EaglePlanOperation_toString(op);
    CUNIT_ASSERT_EQUAL_STRING(msg, "some description");
    EaglePlanOperation_Delete(op);
    EagleMemory_Free(msg);
}

CUNIT_TEST(MainSuite, EaglePlan_Delete)
{
    EaglePlan_Delete(NULL);
}

CUNIT_TEST(MainSuite, EaglePlan_toString)
{
    // test NULL input
    CUNIT_VERIFY_NULL(EaglePlan_toString(NULL));
    
    EaglePlan *plan = EaglePlan_New(0);
    char *msg = (char*) EaglePlan_toString(plan);
    CUNIT_ASSERT_EQUAL_STRING(msg, "EaglePlan:\n");
    EagleMemory_Free(msg);
    
    // add some buffer providers
    EaglePageProvider *provider = EaglePageProvider_CreateFromIntArray(NULL, 0, 10, NULL);
    EaglePlanBufferProvider *bp = EaglePlanBufferProvider_New(123, provider, EagleTrue);
    EaglePlan_addBufferProvider(plan, bp, EagleTrue);
    EaglePlan_prepareBuffers(plan, 1);
    
    // add some steps
    EaglePlanOperation *op1, *op2, *op3;
    EaglePlan_addOperation(plan, op1 = EaglePlanOperation_New(EaglePageOperations_GreaterThanInt, 2, 1, -1, NULL, EagleFalse, "Step 1"));
    EaglePlan_addOperation(plan, op2 = EaglePlanOperation_New(EaglePageOperations_LessThanInt,    3, 1, -1, NULL, EagleFalse, "Step 2"));
    EaglePlan_addOperation(plan, op3 = EaglePlanOperation_New(EaglePageOperations_AndPage,        0, 2,  3, NULL, EagleFalse, "Step 3"));
    
    msg = (char*) EaglePlan_toString(plan);
    CUNIT_ASSERT_EQUAL_STRING(msg, "EaglePlan:\n  Providers:\n    destination = 123, name = (null), type = INTEGER\n  Operations:\n    Step 1\n    Step 2\n    Step 3\n  Buffers:\n    0 type=UNKNOWN\n");
    EagleMemory_Free(msg);
    
    EaglePlan_Delete(plan);
    EaglePlanOperation_Delete(op1);
    EaglePlanOperation_Delete(op2);
    EaglePlanOperation_Delete(op3);
}

CUNIT_TEST(MainSuite, EaglePlanBufferProvider_toString)
{
    EaglePageProvider *provider = EaglePageProvider_CreateFromIntArray(NULL, 0, 10, "something");
    EaglePlanBufferProvider *bp = EaglePlanBufferProvider_New(123, provider, EagleTrue);
    char *description = EaglePlanBufferProvider_toString(bp);
    CUNIT_ASSERT_EQUAL_STRING(description, "destination = 123, name = something, type = INTEGER");
    EagleMemory_Free(description);
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
    EaglePageProvider *provider = EaglePageProvider_CreateFromIntArray(NULL, 0, 1, NULL);
    CUNIT_ASSERT_EQUAL_INT(EaglePageProvider_add(provider, NULL), EagleFalse);
    EaglePageProvider_Delete(provider);
}

CUNIT_TEST(MainSuite, EagleLinkedList_New)
{
    EagleLinkedList *list = EagleLinkedList_New();
    CUNIT_ASSERT_EQUAL_INT(EagleLinkedList_length(list), 0);
    
    // try to add a NULL item
    EagleLinkedList_add(list, NULL);
    CUNIT_ASSERT_EQUAL_INT(EagleLinkedList_length(list), 0);
    
    // add some items
    for(int i = 0; i < 10; ++i) {
        EagleLinkedListItem *item = EagleLinkedListItem_New(EagleData_Int(i * 2), EagleTrue, NULL);
        EagleLinkedList_add(list, item);
    }
    CUNIT_ASSERT_EQUAL_INT(EagleLinkedList_length(list), 10);
    
    // validate list (must be FIFO)
    int count = 0;
    for(EagleLinkedListItem *i = EagleLinkedList_begin(list); NULL != i; i = i->next) {
        if(*((int*) i->obj) != count * 2) {
            CUNIT_FAIL("Linked list in bad order (count = %d, i->obj = %d).", count, *((int*) i->obj));
        }
        ++count;
    }
    CUNIT_ASSERT_EQUAL_INT(count, 10);
           
    EagleLinkedList_DeleteWithItems(list);
}

CUNIT_TEST(MainSuite, EaglePageProvider_CreateFromIntStream)
{
    int testDataSize = 5, recordsPerPage = 2;
    int *testData = (int*) calloc(testDataSize, sizeof(int));
    testData[0] = 123;
    testData[1] = 456;
    testData[2] = 789;
    testData[3] = 234;
    testData[4] = 567;
    
    EaglePageProvider *provider = EaglePageProvider_CreateFromStream(EagleDataTypeInteger, recordsPerPage, NULL);
    CUNIT_VERIFY_EQUAL_INT(provider->totalRecords, 0);
    CUNIT_VERIFY_EQUAL_INT(provider->recordsPerPage, recordsPerPage);
    
    // add data
    for(int i = 0; i < testDataSize; ++i) {
        int *ptr = EagleData_Int(testData[i]);
        EaglePageProvider_add(provider, ptr);
        EagleMemory_Free(ptr);
    }
    
    // get first page
    CUNIT_VERIFY_EQUAL_INT(EaglePageProvider_pagesRemaining(provider), 3);
    EaglePage *page1 = EaglePageProvider_nextPage(provider);
    CUNIT_ASSERT_NOT_NULL(page1);
    CUNIT_VERIFY_EQUAL_INT(page1->count, recordsPerPage);
    CUNIT_VERIFY_EQUAL_INT(((int*) page1->data)[0], testData[0]);
    CUNIT_VERIFY_EQUAL_INT(((int*) page1->data)[1], testData[1]);
    CUNIT_VERIFY_EQUAL_INT(page1->recordOffset, 0);
    EaglePage_Delete(page1);
    
    // reset and read from the first page again
    EaglePageProvider_reset(provider);
    
    CUNIT_VERIFY_EQUAL_INT(EaglePageProvider_pagesRemaining(provider), 3);
    page1 = EaglePageProvider_nextPage(provider);
    CUNIT_ASSERT_NOT_NULL(page1);
    CUNIT_VERIFY_EQUAL_INT(page1->count, recordsPerPage);
    CUNIT_VERIFY_EQUAL_INT(((int*) page1->data)[0], testData[0]);
    CUNIT_VERIFY_EQUAL_INT(((int*) page1->data)[1], testData[1]);
    CUNIT_VERIFY_EQUAL_INT(page1->recordOffset, 0);
    EaglePage_Delete(page1);
    
    CUNIT_VERIFY_EQUAL_INT(EaglePageProvider_pagesRemaining(provider), 2);
    EaglePage *page2 = EaglePageProvider_nextPage(provider);
    CUNIT_ASSERT_NOT_NULL(page1);
    CUNIT_VERIFY_EQUAL_INT(page2->count, recordsPerPage);
    CUNIT_VERIFY_EQUAL_INT(((int*) page2->data)[0], testData[2]);
    CUNIT_VERIFY_EQUAL_INT(((int*) page2->data)[1], testData[3]);
    CUNIT_VERIFY_EQUAL_INT(page2->recordOffset, 2);
    EaglePage_Delete(page2);
    
    CUNIT_VERIFY_EQUAL_INT(EaglePageProvider_pagesRemaining(provider), 1);
    EaglePage *page3 = EaglePageProvider_nextPage(provider);
    CUNIT_ASSERT_NOT_NULL(page3);
    CUNIT_VERIFY_EQUAL_INT(page3->count, 1);
    CUNIT_VERIFY_EQUAL_INT(((int*) page3->data)[0], testData[4]);
    CUNIT_VERIFY_EQUAL_INT(page3->recordOffset, 4);
    EaglePage_Delete(page3);
    
    CUNIT_VERIFY_EQUAL_INT(EaglePageProvider_pagesRemaining(provider), 0);
    CUNIT_VERIFY_NULL(EaglePageProvider_nextPage(provider));
    
    // clean up
    EagleMemory_Free(testData);
    EaglePageProvider_Delete(provider);
}

CUNIT_TEST(MainSuite, EaglePlan_getBufferProviderByName)
{
    EaglePlan *plan = EaglePlan_New(1);
    CUNIT_ASSERT_NULL(EaglePlan_getBufferProviderByName(plan, "abc"));
    EaglePlan_Delete(plan);
}

CUNIT_TEST(MainSuite, EagleDataType_nameToType)
{
    CUNIT_VERIFY_EQUAL_INT(EagleDataType_nameToType("bla"), EagleDataTypeUnknown);
}

CUNIT_TEST(MainSuite, EagleInstance_Delete)
{
    EagleInstance_Delete(NULL);
}

CUNIT_TEST(MainSuite, EagleLoggerSeverity_toString)
{
    CUNIT_ASSERT_EQUAL_STRING(EagleLoggerSeverity_toString(EagleLoggerSeverityDebug), "DEBUG");
    CUNIT_ASSERT_EQUAL_STRING(EagleLoggerSeverity_toString(EagleLoggerSeverityInfo), "INFO");
    CUNIT_ASSERT_EQUAL_STRING(EagleLoggerSeverity_toString(EagleLoggerSeverityUserError), "USERERROR");
    CUNIT_ASSERT_EQUAL_STRING(EagleLoggerSeverity_toString(EagleLoggerSeverityError), "ERROR");
    CUNIT_ASSERT_EQUAL_STRING(EagleLoggerSeverity_toString(EagleLoggerSeverityFatal), "FATAL");
}

EagleLogger* GetLogger()
{
    EagleLogger *logger = EagleLogger_New(fopen("log.txt", "w"));
    CUNIT_ASSERT_NOT_NULL(logger);
    if(NULL != logger) {
        CUNIT_ASSERT_NOT_NULL(logger->out);
    }
    return logger;
}

CUNIT_TEST(MainSuite, EagleLogger_log)
{
    EagleLogger *logger = GetLogger();
    EagleLogger_log(logger, EagleLoggerSeverityDebug, "some message");
    if(NULL != logger) {
        CUNIT_ASSERT_EQUAL_INT(logger->totalMessages, 1);
    }
    EagleLogger_Delete(logger);
}

CUNIT_TEST(MainSuite, EagleLogger_logEvent)
{
    EagleLogger *logger = GetLogger();
    EagleLogger_logEvent(logger, NULL);
    EagleLogger_logEvent(NULL, NULL);
    
    EagleLogger_Delete(logger);
}

CUNIT_TEST(MainSuite, EagleLogger_Get)
{
    EagleLogger *logger = EagleLogger_Get();
    CUNIT_ASSERT_NOT_NULL(logger);
}

CUNIT_TEST(MainSuite, EagleLogger_Log)
{
    EagleLogger_Get()->out = NULL;
    EagleLogger_Log(EagleLoggerSeverityDebug, "some message");
}

CUNIT_TEST(MainSuite, EagleLogger_LogEvent)
{
    EagleLogger_LogEvent(NULL);
    EagleLoggerEvent *event = EagleLoggerEvent_New(EagleLoggerSeverityDebug, "bla bla");
    EagleLogger_LogEvent(event);
}

CUNIT_TEST(MainSuite, EaglePage_CopyInt_)
{
    EaglePage *page = EaglePage_CopyInt_(NULL);
    CUNIT_ASSERT_NULL(page);
}

CUNIT_TEST(MainSuite, EaglePage_CopyText_)
{
    EaglePage *page = EaglePage_CopyText_(NULL);
    CUNIT_ASSERT_NULL(page);
}

CUNIT_TEST(MainSuite, EaglePage_toString)
{
    EaglePage *page = EaglePage_New(EagleDataTypeInteger, NULL, 123, 456, 789, EagleFalse);
    CUNIT_ASSERT_NOT_NULL(page);
    char *desc = EaglePage_toString(page);
    CUNIT_ASSERT_EQUAL_STRING(desc, "EaglePage { type = INTEGER, size = 123, count = 456, offset = 789 }");
    free(desc);
    EaglePage_Delete(page);
}

CUNIT_TEST(MainSuite, EaglePage_Copy)
{
    EaglePage *page = EaglePage_New(EagleDataTypeUnknown, NULL, 123, 456, 789, EagleFalse);
    CUNIT_ASSERT_NOT_NULL(page);
    
    EaglePage *page2 = EaglePage_Copy(page);
    CUNIT_ASSERT_NULL(page2);
    
    EaglePage_Delete(page);
}

CUNIT_TEST(MainSuite, EaglePageOperations_SendPageToProvider)
{
    EaglePageProvider *provider = EaglePageProvider_CreateFromStream(EagleDataTypeInteger, 1, "dummy");
    provider->type = EagleDataTypeUnknown;
    EaglePage *source2 = EaglePage_Alloc(EagleDataTypeInteger, 1);
    EaglePageOperations_SendPageToProvider(NULL, NULL, source2, provider);
    
    EaglePageProvider_Delete(provider);
    EaglePage_Delete(source2);
}

CUNIT_TEST(MainSuite, EaglePageProvider_CreateFromInt)
{
    EagleMemory_MockInit();
    EagleMemory_Mock("EaglePageProvider_CreateFromInt.1");
    
    EaglePageProvider *provider = EaglePageProvider_CreateFromInt(0, 1, "bla");
    CUNIT_ASSERT_NULL(provider);
    
    CUNIT_ASSERT_EQUAL_INT(EagleMemory_GetMockInvocations(), 1);
    EagleMemory_MockFinish();
}

CUNIT_TEST(MainSuite, EaglePageProvider_addStream_)
{
    EaglePageProvider *provider = EaglePageProvider_CreateFromStream(EagleDataTypeInteger, 10, "dummy");
    
    // before we change to Unknown we need to add one record so that the first page is initialised
    int *data = EagleData_Int(123);
    EaglePageProvider_addStream_(provider, data);
    free(data);
    CUNIT_ASSERT_EQUAL_INT(EaglePageProvider_pagesRemaining(provider), 1);
    
    // now skrew up the type
    provider->type = EagleDataTypeUnknown;
    EaglePageProvider_addStream_(provider, NULL);
    
    EaglePageProvider_Delete(provider);
}

CUNIT_TEST(MainSuite, EaglePageProvider_CreateFromStream)
{
    EaglePageProvider *p = EaglePageProvider_CreateFromStream(EagleDataTypeUnknown, 1, "dummy");
    CUNIT_ASSERT_NULL(p);
}

CUNIT_TEST(MainSuite, EaglePlanJob_New)
{
    EaglePlanJob *p = EaglePlanJob_New(NULL);
    CUNIT_ASSERT_NULL(p);
}

CUNIT_TEST(MainSuite, EaglePlan_prepareBuffers)
{
    EaglePlan_prepareBuffers(NULL, 1);
}

CUNIT_TEST(MainSuite, EaglePlan_getExecutionSeconds)
{
    EaglePlan *p = EaglePlan_New(1);
    
    double time = EaglePlan_getExecutionSeconds(p);
    CUNIT_VERIFY_EQUAL_DOUBLE(0.0, time);
    
    p->executionTime = 100000;
    time = EaglePlan_getExecutionSeconds(p);
    CUNIT_VERIFY_EQUAL_DOUBLE(0.0001, time);
    
    EaglePlan_Delete(p);
}

CUNIT_TEST(MainSuite, EagleInstance_nextJob)
{
    EaglePageProvider *provider = EaglePageProvider_CreateFromStream(EagleDataTypeInteger, 1, "dummy");
    int *ptr = EagleData_Int(123);
    EaglePageProvider_add(provider, ptr);
    free(ptr);
    
    EaglePlan *p = EaglePlan_New(1);
    EaglePlanBufferProvider *bp = EaglePlanBufferProvider_New(0, provider, EagleFalse);
    EaglePlan_addBufferProvider(p, bp, EagleFalse);
    
    EagleInstance *instance = EagleInstance_New(1);
    EagleInstance_addPlan(instance, p);
    EaglePlanJob *job = EagleInstance_nextJob(instance);
    CUNIT_VERIFY_NULL(job);
    
    EagleInstance_Delete(instance);
    EaglePlan_Delete(p);
    EaglePageProvider_Delete(provider);
    EaglePlanBufferProvider_Delete(bp);
    EaglePlanJob_Delete(job);
}

CUNIT_TEST(MainSuite, EagleWorker_runJob)
{
    // redirect the errors to nowhere
    EagleLogger_Get()->out = NULL;
    
    EaglePlan *plan = EaglePlan_New(1);
    
    EaglePlanOperation *op = EaglePlanOperation_New(EaglePageOperations_SendPageToProvider, 1, 1, 1, NULL, EagleFalse, "1");
    EaglePlan_addOperation(plan, op);
    EaglePlan_prepareBuffers(plan, 1);
    
    EaglePlanJob *job = EaglePlanJob_New(plan);
    
    EagleWorker_runJob(job);
    CUNIT_ASSERT_NOT_NULL(EagleLogger_Get()->lastEvent);
    CUNIT_VERIFY_EQUAL_STRING(EagleLogger_Get()->lastEvent->message, "destination 1 is greater than allowed 1 buffers!");
    
    op->destination = 0;
    EagleWorker_runJob(job);
    CUNIT_ASSERT_NOT_NULL(EagleLogger_Get()->lastEvent);
    CUNIT_VERIFY_EQUAL_STRING(EagleLogger_Get()->lastEvent->message, "source1 1 is greater than allowed 1 buffers!");
    
    op->source1 = 0;
    EagleWorker_runJob(job);
    CUNIT_ASSERT_NOT_NULL(EagleLogger_Get()->lastEvent);
    CUNIT_VERIFY_EQUAL_STRING(EagleLogger_Get()->lastEvent->message, "source2 1 is greater than allowed 1 buffers!");
    
    EaglePlanOperation_Delete(op);
    EaglePlanJob_Delete(job);
    EaglePlan_Delete(plan);
}

CUNIT_TEST(MainSuite, EagleLogger_LastEvent)
{
    // redirect the errors to nowhere
    EagleLogger_Get()->out = NULL;
    
    EagleLogger_Log(EagleLoggerSeverityDebug, "some message 123");
    
    EagleLoggerEvent *event = EagleLogger_LastEvent();
    CUNIT_ASSERT_NOT_NULL(event);
    if(NULL != event) {
        CUNIT_VERIFY_EQUAL_STRING(event->message, "some message 123");
    }
}

CUNIT_TEST(MainSuite, EagleLogger_lastEvent)
{
    // redirect the errors to nowhere
    EagleLogger_Get()->out = NULL;
    
    CUNIT_VERIFY_NULL(EagleLogger_lastEvent(NULL));
    
    EagleLogger *logger = GetLogger();
    EagleLogger_log(logger, EagleLoggerSeverityDebug, "some message 456");
    
    EagleLoggerEvent *event = EagleLogger_lastEvent(logger);
    CUNIT_ASSERT_NOT_NULL(event);
    if(NULL != event) {
        CUNIT_VERIFY_EQUAL_STRING(event->message, "some message 456");
    }
    
    EagleLogger_Delete(logger);
}

CUNIT_TEST(MainSuite, EagleMemory_MultiFree)
{
    void **data = EagleMemory_MultiAllocate(NULL, 16, 16);
    CUNIT_ASSERT_NOT_NULL(data);
    EagleMemory_MultiFree(data, 16);
}

CUNIT_TEST(MainSuite, EagleMemory_Allocate)
{
    void *data = EagleMemory_Allocate(NULL, SIZE_MAX);
    CUNIT_ASSERT_NULL(data);
}

CUNIT_TEST(MainSuite, EagleLinkedList_pop)
{
    EagleLinkedList *list = EagleLinkedList_New();
    
    // empty list
    {
        CUNIT_VERIFY_EQUAL_INT(EagleLinkedList_length(list), 0);
        CUNIT_VERIFY_EQUAL_INT(EagleLinkedList_isEmpty(list), EagleTrue);
        CUNIT_VERIFY_EQUAL_PTR(EagleLinkedList_begin(list), NULL);
        CUNIT_VERIFY_EQUAL_PTR(EagleLinkedList_end(list), NULL);
        
        EagleLinkedListItem *item = EagleLinkedList_pop(list);
        CUNIT_ASSERT_NULL(item);
        CUNIT_VERIFY_EQUAL_INT(EagleLinkedList_length(list), 0);
        CUNIT_VERIFY_EQUAL_INT(EagleLinkedList_isEmpty(list), EagleTrue);
        CUNIT_VERIFY_EQUAL_PTR(EagleLinkedList_begin(list), NULL);
        CUNIT_VERIFY_EQUAL_PTR(EagleLinkedList_end(list), NULL);
    }
    
    // one item
    {
        EagleLinkedListItem *origItem = EagleLinkedListItem_New(EagleData_Int(123), EagleTrue, NULL);
        EagleLinkedList_add(list, origItem);
        CUNIT_VERIFY_EQUAL_INT(EagleLinkedList_length(list), 1);
        CUNIT_VERIFY_EQUAL_INT(EagleLinkedList_isEmpty(list), EagleFalse);
        CUNIT_VERIFY_EQUAL_PTR(EagleLinkedList_begin(list), origItem);
        CUNIT_VERIFY_EQUAL_PTR(EagleLinkedList_end(list), origItem);
        
        EagleLinkedListItem *item = EagleLinkedList_pop(list);
        
        CUNIT_VERIFY_EQUAL_INT(EagleLinkedList_length(list), 0);
        CUNIT_VERIFY_EQUAL_INT(EagleLinkedList_isEmpty(list), EagleTrue);
        CUNIT_VERIFY_EQUAL_PTR(EagleLinkedList_begin(list), NULL);
        CUNIT_VERIFY_EQUAL_PTR(EagleLinkedList_end(list), NULL);
        CUNIT_VERIFY_EQUAL_INT(*((int*) item->obj), 123);
        
        EagleLinkedListItem_Delete(origItem);
    }
    
    // two items
    {
        EagleLinkedListItem *item;
        EagleLinkedListItem *origItem1 = EagleLinkedListItem_New(EagleData_Int(234), EagleTrue, NULL);
        EagleLinkedListItem *origItem2 = EagleLinkedListItem_New(EagleData_Int(567), EagleTrue, NULL);
        
        EagleLinkedList_add(list, origItem1);
        EagleLinkedList_add(list, origItem2);
        CUNIT_VERIFY_EQUAL_INT(EagleLinkedList_length(list), 2);
        CUNIT_VERIFY_EQUAL_INT(EagleLinkedList_isEmpty(list), EagleFalse);
        CUNIT_VERIFY_EQUAL_PTR(EagleLinkedList_begin(list), origItem1);
        CUNIT_VERIFY_EQUAL_PTR(EagleLinkedList_end(list), origItem2);
        
        item = EagleLinkedList_pop(list);
        CUNIT_VERIFY_EQUAL_INT(EagleLinkedList_length(list), 1);
        CUNIT_VERIFY_EQUAL_INT(EagleLinkedList_isEmpty(list), EagleFalse);
        CUNIT_VERIFY_EQUAL_INT(*((int*) item->obj), 567);
        CUNIT_VERIFY_EQUAL_PTR(EagleLinkedList_begin(list), origItem1);
        CUNIT_VERIFY_EQUAL_PTR(EagleLinkedList_end(list), origItem1);
        
        item = EagleLinkedList_pop(list);
        CUNIT_VERIFY_EQUAL_INT(EagleLinkedList_length(list), 0);
        CUNIT_VERIFY_EQUAL_INT(EagleLinkedList_isEmpty(list), EagleTrue);
        CUNIT_VERIFY_EQUAL_PTR(EagleLinkedList_begin(list), NULL);
        CUNIT_VERIFY_EQUAL_PTR(EagleLinkedList_end(list), NULL);
        CUNIT_VERIFY_EQUAL_INT(*((int*) item->obj), 234);
        
        EagleLinkedListItem_Delete(origItem1);
        EagleLinkedListItem_Delete(origItem2);
    }
    
    // several items
    {
        EagleLinkedListItem *item;
        EagleLinkedListItem *origItem1 = EagleLinkedListItem_New(EagleData_Int(345), EagleTrue, NULL);
        EagleLinkedListItem *origItem2 = EagleLinkedListItem_New(EagleData_Int(678), EagleTrue, NULL);
        EagleLinkedListItem *origItem3 = EagleLinkedListItem_New(EagleData_Int(901), EagleTrue, NULL);
        
        EagleLinkedList_add(list, origItem1);
        EagleLinkedList_add(list, origItem2);
        EagleLinkedList_add(list, origItem3);
        CUNIT_VERIFY_EQUAL_INT(EagleLinkedList_length(list), 3);
        CUNIT_VERIFY_EQUAL_INT(EagleLinkedList_isEmpty(list), EagleFalse);
        CUNIT_VERIFY_EQUAL_PTR(EagleLinkedList_begin(list), origItem1);
        CUNIT_VERIFY_EQUAL_PTR(EagleLinkedList_end(list), origItem3);
        CUNIT_VERIFY_NULL(EagleLinkedList_end(list)->next);
        
        item = EagleLinkedList_pop(list);
        CUNIT_VERIFY_EQUAL_INT(EagleLinkedList_length(list), 2);
        CUNIT_VERIFY_EQUAL_INT(EagleLinkedList_isEmpty(list), EagleFalse);
        CUNIT_VERIFY_EQUAL_PTR(EagleLinkedList_begin(list), origItem1);
        CUNIT_VERIFY_EQUAL_PTR(EagleLinkedList_end(list), origItem2);
        CUNIT_VERIFY_EQUAL_INT(*((int*) item->obj), 901);
        CUNIT_VERIFY_NULL(EagleLinkedList_end(list)->next);
        
        item = EagleLinkedList_pop(list);
        CUNIT_VERIFY_EQUAL_INT(EagleLinkedList_length(list), 1);
        CUNIT_VERIFY_EQUAL_INT(EagleLinkedList_isEmpty(list), EagleFalse);
        CUNIT_VERIFY_EQUAL_PTR(EagleLinkedList_begin(list), origItem1);
        CUNIT_VERIFY_EQUAL_PTR(EagleLinkedList_end(list), origItem1);
        CUNIT_VERIFY_EQUAL_INT(*((int*) item->obj), 678);
        CUNIT_VERIFY_NULL(EagleLinkedList_end(list)->next);
        
        item = EagleLinkedList_pop(list);
        CUNIT_VERIFY_EQUAL_INT(EagleLinkedList_length(list), 0);
        CUNIT_VERIFY_EQUAL_INT(EagleLinkedList_isEmpty(list), EagleTrue);
        CUNIT_VERIFY_EQUAL_PTR(EagleLinkedList_begin(list), NULL);
        CUNIT_VERIFY_EQUAL_PTR(EagleLinkedList_end(list), NULL);
        CUNIT_VERIFY_EQUAL_INT(*((int*) item->obj), 345);
        
        EagleLinkedListItem_Delete(origItem1);
        EagleLinkedListItem_Delete(origItem2);
        EagleLinkedListItem_Delete(origItem3);
    }
    
    EagleLinkedList_Delete(list);
}

CUNIT_TEST(MainSuite, EagleLinkedList_toArray)
{
    int size;
    CUNIT_VERIFY_NULL(EagleLinkedList_toArray(NULL, &size));
    
    EagleLinkedList *list = EagleLinkedList_New();
    EagleLinkedList_add(list, EagleLinkedListItem_New(EagleData_Int(345), EagleTrue, NULL));
    EagleLinkedList_add(list, EagleLinkedListItem_New(EagleData_Int(678), EagleTrue, NULL));
    EagleLinkedList_add(list, EagleLinkedListItem_New(EagleData_Int(901), EagleTrue, NULL));
    
    int **data = (int**) EagleLinkedList_toArray(list, &size);
    CUNIT_ASSERT_EQUAL_INT(size, 3);
    CUNIT_VERIFY_EQUAL_INT(*data[0], 345);
    CUNIT_VERIFY_EQUAL_INT(*data[1], 678);
    CUNIT_VERIFY_EQUAL_INT(*data[2], 901);
    free(data);
    
    EagleLinkedList_DeleteWithItems(list);
}

CUNIT_TEST(MainSuite, EagleLinkedList_add)
{
    EagleLinkedList_add(NULL, NULL);
}

CUNIT_TEST(MainSuite, EagleLinkedList_get)
{
    EagleLinkedList *list = EagleLinkedList_New();
    EagleLinkedList_add(list, EagleLinkedListItem_New(EagleData_Int(345), EagleTrue, NULL));
    
    CUNIT_VERIFY_NULL(EagleLinkedList_get(list, -1));
    CUNIT_VERIFY_NULL(EagleLinkedList_get(list, 1));
    
    EagleLinkedList_DeleteWithItems(list);
}

CUNIT_TEST(MainSuite, EaglePageProvider_CreateFromInt2)
{
    EagleMemory_MockInit();
    EagleMemory_Mock("EaglePageProvider_CreateFromInt.1");
    
    CUNIT_ASSERT_NULL(EaglePageProvider_CreateFromInt(0, 1, NULL));
    
    CUNIT_ASSERT_EQUAL_INT(EagleMemory_GetMockInvocations(), 1);
    EagleMemory_MockFinish();
}

CUNIT_TEST(MainSuite, EagleLinkedList_DeleteItems)
{
    EagleLinkedList_DeleteItems(NULL);
}

CUNIT_TEST(MainSuite, EagleWorker_Delete)
{
    EagleWorker_Delete(NULL);
}

CUNIT_TEST(MainSuite, EaglePageProvider_DeleteIntArray_)
{
    EaglePageProvider_DeleteIntArray_(NULL);
}

CUNIT_TEST(MainSuite, EaglePageProvider_DeleteInt_)
{
    EaglePageProvider_DeleteInt_(NULL);
}

CUNIT_TEST(MainSuite, EaglePageProvider_DeleteStream_)
{
    EaglePageProvider_DeleteStream_(NULL);
}

CUNIT_TEST(MainSuite, EagleWorkers_Delete)
{
    EagleWorkers_Delete(NULL);
}

CUNIT_TEST(MainSuite, EaglePlanBufferProvider_Delete)
{
    EaglePlanBufferProvider_Delete(NULL);
}

CUNIT_TEST(MainSuite, EaglePlanOperation_Delete)
{
    EaglePlanOperation_Delete(NULL);
}

CUNIT_TEST(MainSuite, EagleLock_Delete)
{
    EagleLock_Delete(NULL);
}

CUNIT_TEST(MainSuite, EagleLinkedList_deleteObject1)
{
    int *obj1 = EagleData_Int(123);
    int *obj2 = EagleData_Int(456);
    EagleLinkedList *list = EagleLinkedList_New();
    
    // NULL object
    CUNIT_ASSERT_FALSE(EagleLinkedList_deleteObject(list, NULL));
    CUNIT_ASSERT_EQUAL_INT(0, EagleLinkedList_length(list));
    
    // empty list
    CUNIT_ASSERT_FALSE(EagleLinkedList_deleteObject(list, obj1));
    CUNIT_ASSERT_EQUAL_INT(0, EagleLinkedList_length(list));
    
    // one object
    EagleLinkedList_addObject(list, obj1, EagleFalse, NULL);
    CUNIT_ASSERT_EQUAL_INT(1, EagleLinkedList_length(list));
    
    CUNIT_ASSERT_FALSE(EagleLinkedList_deleteObject(list, obj2));
    CUNIT_ASSERT_EQUAL_INT(1, EagleLinkedList_length(list));
    CUNIT_ASSERT_NOT_NULL(list->first);
    CUNIT_ASSERT_NOT_NULL(list->last);
    
    CUNIT_ASSERT_TRUE(EagleLinkedList_deleteObject(list, obj1));
    CUNIT_ASSERT_EQUAL_INT(0, EagleLinkedList_length(list));
    CUNIT_ASSERT_NULL(list->first);
    CUNIT_ASSERT_NULL(list->last);
    
    EagleLinkedList_DeleteWithItems(list);
    EagleMemory_Free(obj1);
    EagleMemory_Free(obj2);
}

CUNIT_TEST(MainSuite, EagleLinkedList_deleteObject2)
{
    int *obj1 = EagleData_Int(123);
    int *obj2 = EagleData_Int(456);
    int *obj3 = EagleData_Int(789);
    int *obj4 = EagleData_Int(789);
    EagleLinkedList *list = EagleLinkedList_New();
    
    // add objects
    EagleLinkedList_addObject(list, obj1, EagleTrue, NULL);
    EagleLinkedList_addObject(list, obj2, EagleTrue, NULL);
    EagleLinkedList_addObject(list, obj3, EagleTrue, NULL);
    CUNIT_ASSERT_EQUAL_INT(3, EagleLinkedList_length(list));
    CUNIT_ASSERT_EQUAL_PTR(list->first->obj, obj1);
    CUNIT_ASSERT_EQUAL_PTR(list->last->obj, obj3);
    
    // object that doesn't exist
    CUNIT_ASSERT_FALSE(EagleLinkedList_deleteObject(list, obj4));
    CUNIT_ASSERT_EQUAL_INT(3, EagleLinkedList_length(list));
    CUNIT_ASSERT_NOT_NULL(list->first);
    CUNIT_ASSERT_NOT_NULL(list->last);
    
    // first object
    CUNIT_ASSERT_TRUE(EagleLinkedList_deleteObject(list, obj1));
    CUNIT_ASSERT_EQUAL_INT(2, EagleLinkedList_length(list));
    CUNIT_ASSERT_NOT_NULL(list->first);
    CUNIT_ASSERT_NOT_NULL(list->last);
    CUNIT_ASSERT_EQUAL_PTR(list->first->obj, obj2);
    CUNIT_ASSERT_EQUAL_PTR(list->last->obj, obj3);
    
    EagleLinkedList_DeleteWithItems(list);
    EagleMemory_Free(obj4);
}

CUNIT_TEST(MainSuite, EagleLinkedList_deleteObject3)
{
    int *obj1 = EagleData_Int(123);
    int *obj2 = EagleData_Int(456);
    int *obj3 = EagleData_Int(789);
    EagleLinkedList *list = EagleLinkedList_New();
    
    // add objects
    EagleLinkedList_addObject(list, obj1, EagleTrue, NULL);
    EagleLinkedList_addObject(list, obj2, EagleTrue, NULL);
    EagleLinkedList_addObject(list, obj3, EagleTrue, NULL);
    CUNIT_ASSERT_EQUAL_INT(3, EagleLinkedList_length(list));
    CUNIT_ASSERT_EQUAL_PTR(list->first->obj, obj1);
    CUNIT_ASSERT_EQUAL_PTR(list->last->obj, obj3);
    
    // middle object
    CUNIT_ASSERT_TRUE(EagleLinkedList_deleteObject(list, obj2));
    CUNIT_ASSERT_EQUAL_INT(2, EagleLinkedList_length(list));
    CUNIT_ASSERT_NOT_NULL(list->first);
    CUNIT_ASSERT_NOT_NULL(list->last);
    CUNIT_ASSERT_EQUAL_PTR(list->first->obj, obj1);
    CUNIT_ASSERT_EQUAL_PTR(list->last->obj, obj3);
    
    EagleLinkedList_DeleteWithItems(list);
}

CUNIT_TEST(MainSuite, EagleLinkedList_deleteObject4)
{
    int *obj1 = EagleData_Int(123);
    int *obj2 = EagleData_Int(456);
    int *obj3 = EagleData_Int(789);
    EagleLinkedList *list = EagleLinkedList_New();
    
    // add objects
    EagleLinkedList_addObject(list, obj1, EagleTrue, NULL);
    EagleLinkedList_addObject(list, obj2, EagleTrue, NULL);
    EagleLinkedList_addObject(list, obj3, EagleTrue, NULL);
    CUNIT_ASSERT_EQUAL_INT(3, EagleLinkedList_length(list));
    CUNIT_ASSERT_EQUAL_PTR(list->first->obj, obj1);
    CUNIT_ASSERT_EQUAL_PTR(list->last->obj, obj3);
    
    // last object
    CUNIT_ASSERT_TRUE(EagleLinkedList_deleteObject(list, obj3));
    CUNIT_ASSERT_EQUAL_INT(2, EagleLinkedList_length(list));
    CUNIT_ASSERT_NOT_NULL(list->first);
    CUNIT_ASSERT_NOT_NULL(list->last);
    CUNIT_ASSERT_EQUAL_PTR(list->first->obj, obj1);
    CUNIT_ASSERT_EQUAL_PTR(list->last->obj, obj2);
    
    EagleLinkedList_DeleteWithItems(list);
}

CUnitTests* MainSuite_tests()
{
    CUnitTests *tests = CUnitTests_New(100);
    
    // method tests
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EagleLinkedList_deleteObject4));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EagleLinkedList_deleteObject3));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EagleLinkedList_deleteObject2));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EagleLinkedList_deleteObject1));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EagleLock_Delete));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePlanOperation_Delete));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePlanBufferProvider_Delete));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EagleWorkers_Delete));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePageProvider_DeleteStream_));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePageProvider_DeleteInt_));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePageProvider_DeleteIntArray_));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EagleWorker_Delete));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EagleLinkedList_DeleteItems));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePageProvider_CreateFromInt2));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EagleLinkedList_get));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EagleLinkedList_add));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EagleLinkedList_toArray));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EagleLinkedList_pop));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EagleMemory_Allocate));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EagleMemory_MultiFree));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EagleLogger_LastEvent));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EagleLogger_lastEvent));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EagleWorker_runJob));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EagleInstance_nextJob));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePlan_getExecutionSeconds));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePlan_prepareBuffers));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EagleDataType_nameToType));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EagleInstance_Delete));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EagleLinkedList_New));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePageOperations_GreaterThanInt));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePageOperations_LessThanInt));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePageOperations_AndPage));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePageProvider_TotalPages));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePageProvider_CreateFromIntArray));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePageProvider_CreateFromIntStream));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePageProvider_Delete));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePageProvider_add));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePlan_Delete));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePlan_toString));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePlan_getBufferProviderByName));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePlanBufferProvider_toString));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePlanJob_Delete));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePlanOperation_toString));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EagleLoggerSeverity_toString));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EagleLogger_log));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EagleLogger_logEvent));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EagleLogger_Get));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EagleLogger_Log));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EagleLogger_LogEvent));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePage_CopyInt_));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePage_CopyText_));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePage_toString));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePage_Copy));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePageOperations_SendPageToProvider));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePageProvider_CreateFromInt));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePageProvider_addStream_));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePageProvider_CreateFromStream));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePlanJob_New));
    
    // complex / execution tests
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, _, InstanceSingle));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, _, InstanceMulti));
    
    return tests;
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
