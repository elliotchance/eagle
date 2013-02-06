#include "OperatorSuite.h"
#include <stdio.h>
#include <stdlib.h>
#include "EaglePageOperations.h"
#include "EagleMemory.h"
#include "EagleDbInstance.h"
#include "EagleInstance.h"
#include "EagleDbParser.h"

EaglePage* OperatorSuite_GeneratePage(int pageSize)
{
    EaglePage *page = EaglePage_AllocInt(pageSize);
    
    // prepare
    for(int i = 0; i < pageSize; ++i) {
        ((int*) page->data)[i] = i;
    }
    
    return page;
}

u_int32_t arc4random0(u_int32_t max)
{
    while(1) {
        u_int32_t r = arc4random() % max;
        if(0 != r) {
            return r;
        }
    }
}

CUNIT_TEST(OperatorSuite, EaglePageOperations_GreaterThanInt)
{
    int pageSize = 1000;
    int testValue = arc4random();
    EaglePage *page = OperatorSuite_GeneratePage(pageSize);
    EaglePage *out = EaglePage_AllocInt(pageSize);
    int *int1 = EagleData_Int(testValue);
    
    EaglePageOperations_GreaterThanInt(out, page, NULL, int1);
    
    int valid = 1;
    for(int i = 0; i < pageSize; ++i) {
        if(!(((int*) page->data)[i] > testValue == ((int*) out->data)[i])) {
            valid = 0;
            break;
        }
    }
    CUNIT_ASSERT_EQUAL_INT(valid, 1);
    
    EagleMemory_Free(int1);
    EaglePage_Delete(page);
    EaglePage_Delete(out);
}

CUNIT_TEST(OperatorSuite, EaglePageOperations_LessThanInt)
{
    int pageSize = 1000;
    int testValue = arc4random();
    EaglePage *page = OperatorSuite_GeneratePage(pageSize);
    EaglePage *out = EaglePage_AllocInt(pageSize);
    int *int1 = EagleData_Int(testValue);
    
    EaglePageOperations_LessThanInt(out, page, NULL, int1);
    
    int valid = 1;
    for(int i = 0; i < pageSize; ++i) {
        if(!(((int*) page->data)[i] < testValue == ((int*) out->data)[i])) {
            valid = 0;
            break;
        }
    }
    CUNIT_ASSERT_EQUAL_INT(valid, 1);
    
    EagleMemory_Free(int1);
    EaglePage_Delete(page);
    EaglePage_Delete(out);
}

CUNIT_TEST(OperatorSuite, EaglePageOperations_AndPage)
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
    
    int valid = 1;
    for(int i = 0; i < pageSize; ++i) {
        if(((int*) page1->data)[i] && ((int*) page2->data)[i] != ((int*) out->data)[i]) {
            valid = 0;
            break;
        }
    }
    CUNIT_ASSERT_EQUAL_INT(valid, 1);
    
    EaglePage_Delete(page1);
    EaglePage_Delete(page2);
    EaglePage_Delete(out);
}

CUNIT_TEST(OperatorSuite, EaglePageOperations_AdditionPage)
{
    int pageSize = 1000;
    EaglePage *page1 = EaglePage_AllocInt(pageSize);
    EaglePage *page2 = EaglePage_AllocInt(pageSize);
    EaglePage *out = EaglePage_AllocInt(pageSize);
    for(int i = 0; i < pageSize; ++i) {
        ((int*) page1->data)[i] = arc4random() % 2;
        ((int*) page2->data)[i] = arc4random() % 2;
    }
    
    EaglePageOperations_AdditionPage(out, page1, page2, NULL);
    
    int valid = 1;
    for(int i = 0; i < pageSize; ++i) {
        if(((int*) page1->data)[i] + ((int*) page2->data)[i] != ((int*) out->data)[i]) {
            valid = 0;
            break;
        }
    }
    CUNIT_ASSERT_EQUAL_INT(valid, 1);
    
    EaglePage_Delete(page1);
    EaglePage_Delete(page2);
    EaglePage_Delete(out);
}

CUNIT_TEST(OperatorSuite, EaglePageOperations_EqualsPage)
{
    int pageSize = 1000;
    EaglePage *page1 = EaglePage_AllocInt(pageSize);
    EaglePage *page2 = EaglePage_AllocInt(pageSize);
    EaglePage *out = EaglePage_AllocInt(pageSize);
    for(int i = 0; i < pageSize; ++i) {
        ((int*) page1->data)[i] = arc4random() % 2;
        ((int*) page2->data)[i] = arc4random() % 2;
    }
    
    EaglePageOperations_EqualsPage(out, page1, page2, NULL);
    
    int valid = 1;
    for(int i = 0; i < pageSize; ++i) {
        if((((int*) page1->data)[i] == ((int*) page2->data)[i]) != ((int*) out->data)[i]) {
            valid = 0;
            break;
        }
    }
    CUNIT_ASSERT_EQUAL_INT(valid, 1);
    
    EaglePage_Delete(page1);
    EaglePage_Delete(page2);
    EaglePage_Delete(out);
}

CUNIT_TEST(OperatorSuite, EaglePageOperations_ModulusPage)
{
    int pageSize = 1000;
    EaglePage *page1 = EaglePage_AllocInt(pageSize);
    EaglePage *page2 = EaglePage_AllocInt(pageSize);
    EaglePage *out = EaglePage_AllocInt(pageSize);
    for(int i = 0; i < pageSize; ++i) {
        ((int*) page1->data)[i] = arc4random0(1000);
        ((int*) page2->data)[i] = arc4random0(1000);
    }
    
    EaglePageOperations_ModulusPage(out, page1, page2, NULL);
    
    int valid = 1;
    for(int i = 0; i < pageSize; ++i) {
        if((((int*) page1->data)[i] % ((int*) page2->data)[i]) != ((int*) out->data)[i]) {
            valid = 0;
            break;
        }
    }
    CUNIT_ASSERT_EQUAL_INT(valid, 1);
    
    EaglePage_Delete(page1);
    EaglePage_Delete(page2);
    EaglePage_Delete(out);
}

CUNIT_TEST(OperatorSuite, OperatorPrecedence)
{
    int pageSize = 10;
    EagleDbInstance *db = EagleDbInstance_New(pageSize);
    
    EagleDbSchema *schema = EagleDbSchema_New((char*) EagleDbSchema_DefaultSchemaName);
    EagleDbInstance_addSchema(db, schema);
    
    EagleDbTable *table = EagleDbTable_New("mytable");
    EagleDbTable_addColumn(table, EagleDbColumn_New("col1", EagleDataTypeInteger));
    
    EagleDbTableData *td = EagleDbTableData_New(table, pageSize);
    EagleDbSchema_addTable(schema, td);
    
    for(int i = 0; i < 10; ++i) {
        // create a record
        EagleDbTuple *tuple = EagleDbTuple_New(table);
        EagleDbTuple_setInt(tuple, 0, i);
        
        // put record in
        EagleDbTableData_insert(td, tuple);
    }
    
    /* parse sql */
    EagleDbParser_Init();
    EagleDbParser_LoadString("SELECT col1 * 2 + 10 * 2 FROM mytable;");
    EagleDbParser_Parse();
    
    if(EagleTrue == EagleDbParser_HasError()) {
        CUNIT_FAIL("%s", EagleDbParser_LastError());
    }
    
    /* compile plan */
    EaglePlan *plan = EagleDbSqlSelect_parse((EagleDbSqlSelect*) EagleDbParser_Get()->yyparse_ast, db);
    CUNIT_ASSERT_NOT_NULL(plan);
    CUNIT_ASSERT_FALSE(EaglePlan_isError(plan));
    
    EagleInstance *eagle = EagleInstance_New(1);
    EagleInstance_addPlan(eagle, plan);
    EagleInstance_run(eagle);
    
    /* check results */
    EaglePage *page = EaglePageProvider_nextPage(plan->result[0]);
    CUNIT_ASSERT_NOT_NULL(page);
    
    for(int i = 0; i < page->count; ++i) {
        printf("%d\n", ((int*) page->data)[i]);
    }
}

CUnitTests* OperatorSuite_tests()
{
    CUnitTests *tests = CUnitTests_New(100);
    
    // method tests
    //CUnitTests_addTest(tests, CUNIT_NEW(OperatorSuite, OperatorPrecedence));
    CUnitTests_addTest(tests, CUNIT_NEW_NAME(OperatorSuite, EaglePageOperations_GreaterThanInt, "int > int"));
    CUnitTests_addTest(tests, CUNIT_NEW_NAME(OperatorSuite, EaglePageOperations_GreaterThanInt, "int < int"));
    CUnitTests_addTest(tests, CUNIT_NEW_NAME(OperatorSuite, EaglePageOperations_AndPage, "int AND int"));
    CUnitTests_addTest(tests, CUNIT_NEW_NAME(OperatorSuite, EaglePageOperations_AdditionPage, "int + int"));
    CUnitTests_addTest(tests, CUNIT_NEW_NAME(OperatorSuite, EaglePageOperations_EqualsPage, "int = int"));
    CUnitTests_addTest(tests, CUNIT_NEW_NAME(OperatorSuite, EaglePageOperations_ModulusPage, "int % int"));
    
    return tests;
}

/**
 * The suite init function.
 */
int OperatorSuite_init()
{
    return 0;
}

/**
 * The suite cleanup function.
 */
int OperatorSuite_clean()
{
    return 0;
}
