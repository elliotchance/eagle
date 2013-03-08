#include "MainSuite.h"
#include <stdio.h>
#include <stdlib.h>
#include "EagleData.h"
#include "EaglePageOperations.h"
#include "EagleInstance.h"
#include "EagleLinkedList.h"
#include "EagleMemory.h"
#include "EagleLogger.h"
#include "EagleUtils.h"
#include "EaglePageProviderStream.h"
#include "EaglePageProviderArray.h"
#include "EaglePageProviderSingle.h"
#include "EaglePageProviderVirtual.h"

CUNIT_TEST(MainSuite, EaglePageProvider_reset)
{
    EaglePageProviderSingle *epp = EaglePageProviderSingle_NewInt(123, 1, "bla");
    EaglePageProvider_reset((EaglePageProvider*) epp);
    EaglePageProviderSingle_Delete(epp);
}

CUNIT_TEST(MainSuite, EaglePageProviderVirtual_New)
{
    // create a virtual provider that acts like a single provider
    EaglePageProviderSingle *single = EaglePageProviderSingle_NewInt(123, 1, "name");
    EaglePageProviderVirtual *epp = EaglePageProviderVirtual_New(single->recordsPerPage,
         "name2",
         EagleDataTypeInteger,
         single,
         (void (*)(void*)) EaglePageProvider_Delete,
         (EagleBoolean (*)(void*, void*)) EaglePageProvider_add,
         (int (*)(void*)) EaglePageProvider_pagesRemaining,
         (EaglePage* (*)(void*)) EaglePageProvider_nextPage,
         (void (*)(void*)) EaglePageProvider_reset);
    
    // add
    CUNIT_VERIFY_EQUAL_INT(EaglePageProvider_add((EaglePageProvider*) epp, NULL), EaglePageProvider_add((EaglePageProvider*) single, NULL));
    
    // pagesRemaining
    CUNIT_VERIFY_EQUAL_INT(EaglePageProvider_pagesRemaining((EaglePageProvider*) epp), EaglePageProvider_pagesRemaining((EaglePageProvider*) single));
    
    // nextPage
    EaglePage *page1 = EaglePageProvider_nextPage((EaglePageProvider*) epp);
    EaglePage *page2 = EaglePageProvider_nextPage((EaglePageProvider*) single);
    CUNIT_VERIFY_EQUAL_INT(page1->count, page2->count);
    CUNIT_VERIFY_EQUAL_INT(page1->totalSize, page2->totalSize);
    EaglePage_Delete(page1);
    EaglePage_Delete(page2);
    
    // reset
    EaglePageProvider_reset((EaglePageProvider*) epp);
    EaglePageProvider_reset((EaglePageProvider*) single);
    
    EaglePageProvider_Delete((EaglePageProvider*) epp);
}

CUNIT_TEST(MainSuite, EaglePageProviderVirtual_Delete)
{
    EaglePageProviderVirtual_Delete(NULL);
}

CUNIT_TEST(MainSuite, EaglePageProviderArray_nextPage)
{
    EaglePageProviderArray *epp = EaglePageProviderArray_NewInt(NULL, 0, 1, "name");
    CUNIT_VERIFY_NULL(EaglePageProviderArray_nextPage(epp));
    EaglePageProvider_Delete((EaglePageProvider*) epp);
}

CUNIT_TEST(MainSuite, EaglePageProviderSingle_nextPage_1)
{
    EaglePageProviderSingle *epp = EaglePageProviderSingle_NewInt(123, 1, "name");
    epp->type = EagleDataTypeUnknown;
    CUNIT_VERIFY_NULL(EaglePageProviderSingle_nextPage(epp));
    EaglePageProvider_Delete((EaglePageProvider*) epp);
}

CUNIT_TEST(MainSuite, EaglePageProviderSingle_nextPage_2)
{
    int recordsPerPage = 5;
    EaglePageProviderSingle *epp = EaglePageProviderSingle_NewVarchar("some value", recordsPerPage, "name");
    
    EaglePage *page = EaglePageProviderSingle_nextPage(epp);
    CUNIT_ASSERT_NOT_NULL(page);
    CUNIT_VERIFY_EQUAL_INT(page->count, recordsPerPage);
    EaglePage_Delete(page);
    
    EaglePageProvider_Delete((EaglePageProvider*) epp);
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

CUnitTests* MainSuite2_tests()
{
    CUnitTests *tests = CUnitTests_New(100);
    
    // method tests
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePageProviderSingle_nextPage_2));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePageProviderSingle_nextPage_1));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePageProviderArray_nextPage));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePageProviderVirtual_Delete));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePageProviderVirtual_New));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EaglePageProvider_reset));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EagleLinkedList_deleteObject4));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EagleLinkedList_deleteObject3));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EagleLinkedList_deleteObject2));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EagleLinkedList_deleteObject1));
    
    return tests;
}
