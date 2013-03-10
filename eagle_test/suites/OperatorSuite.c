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

CUNIT_TEST(OperatorSuite, EaglePageOperations_AndPageInt)
{
    int pageSize = 1000;
    EaglePage *page1 = EaglePage_AllocInt(pageSize);
    EaglePage *page2 = EaglePage_AllocInt(pageSize);
    EaglePage *out = EaglePage_AllocInt(pageSize);
    for(int i = 0; i < pageSize; ++i) {
        ((int*) page1->data)[i] = arc4random() % 2;
        ((int*) page2->data)[i] = arc4random() % 2;
    }
    
    EaglePageOperations_AndPageInt(out, page1, page2, NULL);
    
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

CUNIT_TEST(OperatorSuite, EaglePageOperations_OrPageInt)
{
    int pageSize = 1000;
    EaglePage *page1 = EaglePage_AllocInt(pageSize);
    EaglePage *page2 = EaglePage_AllocInt(pageSize);
    EaglePage *out = EaglePage_AllocInt(pageSize);
    for(int i = 0; i < pageSize; ++i) {
        ((int*) page1->data)[i] = arc4random() % 2;
        ((int*) page2->data)[i] = arc4random() % 2;
    }
    
    EaglePageOperations_OrPageInt(out, page1, page2, NULL);
    
    int valid = 1;
    for(int i = 0; i < pageSize; ++i) {
        if((((int*) page1->data)[i] || ((int*) page2->data)[i]) != ((int*) out->data)[i]) {
            valid = 0;
            break;
        }
    }
    CUNIT_ASSERT_EQUAL_INT(valid, 1);
    
    EaglePage_Delete(page1);
    EaglePage_Delete(page2);
    EaglePage_Delete(out);
}

CUNIT_TEST(OperatorSuite, EaglePageOperations_AdditionPageInt)
{
    int pageSize = 1000;
    EaglePage *page1 = EaglePage_AllocInt(pageSize);
    EaglePage *page2 = EaglePage_AllocInt(pageSize);
    EaglePage *out = EaglePage_AllocInt(pageSize);
    for(int i = 0; i < pageSize; ++i) {
        ((int*) page1->data)[i] = arc4random() % 100;
        ((int*) page2->data)[i] = arc4random() % 100;
    }
    
    EaglePageOperations_AdditionPageInt(out, page1, page2, NULL);
    
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

CUNIT_TEST(OperatorSuite, EaglePageOperations_SubtractPageInt)
{
    int pageSize = 1000;
    EaglePage *page1 = EaglePage_AllocInt(pageSize);
    EaglePage *page2 = EaglePage_AllocInt(pageSize);
    EaglePage *out = EaglePage_AllocInt(pageSize);
    for(int i = 0; i < pageSize; ++i) {
        ((int*) page1->data)[i] = arc4random() % 100;
        ((int*) page2->data)[i] = arc4random() % 100;
    }
    
    EaglePageOperations_SubtractPageInt(out, page1, page2, NULL);
    
    int valid = 1;
    for(int i = 0; i < pageSize; ++i) {
        if(((int*) page1->data)[i] - ((int*) page2->data)[i] != ((int*) out->data)[i]) {
            valid = 0;
            break;
        }
    }
    CUNIT_ASSERT_EQUAL_INT(valid, 1);
    
    EaglePage_Delete(page1);
    EaglePage_Delete(page2);
    EaglePage_Delete(out);
}

CUNIT_TEST(OperatorSuite, EaglePageOperations_MultiplyPageInt)
{
    int pageSize = 1000;
    EaglePage *page1 = EaglePage_AllocInt(pageSize);
    EaglePage *page2 = EaglePage_AllocInt(pageSize);
    EaglePage *out = EaglePage_AllocInt(pageSize);
    for(int i = 0; i < pageSize; ++i) {
        ((int*) page1->data)[i] = arc4random() % 100;
        ((int*) page2->data)[i] = arc4random() % 100;
    }
    
    EaglePageOperations_MultiplyPageInt(out, page1, page2, NULL);
    
    int valid = 1;
    for(int i = 0; i < pageSize; ++i) {
        if(((int*) page1->data)[i] * ((int*) page2->data)[i] != ((int*) out->data)[i]) {
            valid = 0;
            break;
        }
    }
    CUNIT_ASSERT_EQUAL_INT(valid, 1);
    
    EaglePage_Delete(page1);
    EaglePage_Delete(page2);
    EaglePage_Delete(out);
}

CUNIT_TEST(OperatorSuite, EaglePageOperations_EqualsPageInt)
{
    int pageSize = 1000;
    EaglePage *page1 = EaglePage_AllocInt(pageSize);
    EaglePage *page2 = EaglePage_AllocInt(pageSize);
    EaglePage *out = EaglePage_AllocInt(pageSize);
    for(int i = 0; i < pageSize; ++i) {
        ((int*) page1->data)[i] = arc4random() % 2;
        ((int*) page2->data)[i] = arc4random() % 2;
    }
    
    EaglePageOperations_EqualsPageInt(out, page1, page2, NULL);
    
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

CUNIT_TEST(OperatorSuite, EaglePageOperations_NotEqualsPageInt)
{
    int pageSize = 1000;
    EaglePage *page1 = EaglePage_AllocInt(pageSize);
    EaglePage *page2 = EaglePage_AllocInt(pageSize);
    EaglePage *out = EaglePage_AllocInt(pageSize);
    for(int i = 0; i < pageSize; ++i) {
        ((int*) page1->data)[i] = arc4random() % 10;
        ((int*) page2->data)[i] = arc4random() % 10;
    }
    
    EaglePageOperations_NotEqualsPageInt(out, page1, page2, NULL);
    
    int valid = 1;
    for(int i = 0; i < pageSize; ++i) {
        if((((int*) page1->data)[i] != ((int*) page2->data)[i]) != ((int*) out->data)[i]) {
            valid = 0;
            break;
        }
    }
    CUNIT_ASSERT_EQUAL_INT(valid, 1);
    
    EaglePage_Delete(page1);
    EaglePage_Delete(page2);
    EaglePage_Delete(out);
}

CUNIT_TEST(OperatorSuite, EaglePageOperations_ModulusPageInt)
{
    int pageSize = 1000;
    EaglePage *page1 = EaglePage_AllocInt(pageSize);
    EaglePage *page2 = EaglePage_AllocInt(pageSize);
    EaglePage *out = EaglePage_AllocInt(pageSize);
    for(int i = 0; i < pageSize; ++i) {
        ((int*) page1->data)[i] = arc4random0(1000);
        ((int*) page2->data)[i] = arc4random0(1000);
    }
    
    // make sure the first one is 0
    ((int*) page2->data)[0] = 0;
    
    EaglePageOperations_ModulusPageInt(out, page1, page2, NULL);
    
    // modulus by 0
    CUNIT_VERIFY_EQUAL_INT(0, ((int*) out->data)[0]);
    
    int valid = 1;
    for(int i = 1; i < pageSize; ++i) {
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

CUNIT_TEST(OperatorSuite, EaglePageOperations_GreaterThanPageInt)
{
    int pageSize = 1000;
    EaglePage *page1 = EaglePage_AllocInt(pageSize);
    EaglePage *page2 = EaglePage_AllocInt(pageSize);
    EaglePage *out = EaglePage_AllocInt(pageSize);
    for(int i = 0; i < pageSize; ++i) {
        ((int*) page1->data)[i] = arc4random() % 100;
        ((int*) page2->data)[i] = arc4random() % 100;
    }
    
    EaglePageOperations_GreaterThanPageInt(out, page1, page2, NULL);
    
    int valid = 1;
    for(int i = 0; i < pageSize; ++i) {
        if((((int*) page1->data)[i] > ((int*) page2->data)[i]) != ((int*) out->data)[i]) {
            valid = 0;
            break;
        }
    }
    CUNIT_ASSERT_EQUAL_INT(valid, 1);
    
    EaglePage_Delete(page1);
    EaglePage_Delete(page2);
    EaglePage_Delete(out);
}

CUNIT_TEST(OperatorSuite, EaglePageOperations_LessThanPageInt)
{
    int pageSize = 1000;
    EaglePage *page1 = EaglePage_AllocInt(pageSize);
    EaglePage *page2 = EaglePage_AllocInt(pageSize);
    EaglePage *out = EaglePage_AllocInt(pageSize);
    for(int i = 0; i < pageSize; ++i) {
        ((int*) page1->data)[i] = arc4random() % 100;
        ((int*) page2->data)[i] = arc4random() % 100;
    }
    
    EaglePageOperations_LessThanPageInt(out, page1, page2, NULL);
    
    int valid = 1;
    for(int i = 0; i < pageSize; ++i) {
        if((((int*) page1->data)[i] < ((int*) page2->data)[i]) != ((int*) out->data)[i]) {
            valid = 0;
            break;
        }
    }
    CUNIT_ASSERT_EQUAL_INT(valid, 1);
    
    EaglePage_Delete(page1);
    EaglePage_Delete(page2);
    EaglePage_Delete(out);
}

CUNIT_TEST(OperatorSuite, EaglePageOperations_GreaterThanEqualPageInt)
{
    int pageSize = 1000;
    EaglePage *page1 = EaglePage_AllocInt(pageSize);
    EaglePage *page2 = EaglePage_AllocInt(pageSize);
    EaglePage *out = EaglePage_AllocInt(pageSize);
    for(int i = 0; i < pageSize; ++i) {
        ((int*) page1->data)[i] = arc4random() % 100;
        ((int*) page2->data)[i] = arc4random() % 100;
    }
    
    EaglePageOperations_GreaterThanEqualPageInt(out, page1, page2, NULL);
    
    int valid = 1;
    for(int i = 0; i < pageSize; ++i) {
        if((((int*) page1->data)[i] >= ((int*) page2->data)[i]) != ((int*) out->data)[i]) {
            valid = 0;
            break;
        }
    }
    CUNIT_ASSERT_EQUAL_INT(valid, 1);
    
    EaglePage_Delete(page1);
    EaglePage_Delete(page2);
    EaglePage_Delete(out);
}

CUNIT_TEST(OperatorSuite, EaglePageOperations_LessThanEqualPageInt)
{
    int pageSize = 1000;
    EaglePage *page1 = EaglePage_AllocInt(pageSize);
    EaglePage *page2 = EaglePage_AllocInt(pageSize);
    EaglePage *out = EaglePage_AllocInt(pageSize);
    for(int i = 0; i < pageSize; ++i) {
        ((int*) page1->data)[i] = arc4random() % 100;
        ((int*) page2->data)[i] = arc4random() % 100;
    }
    
    EaglePageOperations_LessThanEqualPageInt(out, page1, page2, NULL);
    
    int valid = 1;
    for(int i = 0; i < pageSize; ++i) {
        if((((int*) page1->data)[i] <= ((int*) page2->data)[i]) != ((int*) out->data)[i]) {
            valid = 0;
            break;
        }
    }
    CUNIT_ASSERT_EQUAL_INT(valid, 1);
    
    EaglePage_Delete(page1);
    EaglePage_Delete(page2);
    EaglePage_Delete(out);
}

void OperatorSuite_testOperator(EagleDbInstance *db, const char *expr, int result)
{
    char newsql[1024];
    
    /* parse sql */
    sprintf(newsql, "SELECT %s FROM mytable;", expr);
    EagleDbParser *p = EagleDbParser_ParseWithString(newsql);
    if(EagleTrue == EagleDbParser_hasError(p)) {
        CUNIT_FAIL("%s", EagleDbParser_lastError(p));
    }
    
    /* compile plan */
    EaglePlan *plan = EagleDbSqlSelect_parse(p->yyparse_ast, db);
    if(NULL == plan) {
        CUNIT_ASSERT_NOT_NULL(plan);
    }
    if(EagleTrue == EaglePlan_isError(plan)) {
        CUNIT_FAIL("%s", plan->errorMessage);
    }
    
    EagleInstance *eagle = EagleInstance_New(1);
    EagleInstance_addPlan(eagle, plan);
    EagleInstance_run(eagle);
    
    /* check results */
    EaglePage *page = EaglePageProvider_nextPage(plan->result[0]);
    if(NULL == page) {
        CUNIT_ASSERT_NOT_NULL(page);
    }
    
    for(int i = 0; i < page->count; ++i) {
        if(((int*) page->data)[i] != result) {
            CUNIT_FAIL("Expression failed (%s): Expected %d, received %d", expr, result, ((int*) page->data)[i]);
        }
    }
    
    EagleDbSqlExpression_DeleteRecursive(p->yyparse_ast);
    EaglePage_Delete(page);
    EaglePlan_Delete(plan);
    EagleDbParser_Delete(p);
    EagleInstance_Delete(eagle);
}

CUNIT_TEST(OperatorSuite, OperatorPrecedence)
{
    int pageSize = 1;
    EagleDbInstance *db = EagleDbInstance_New(pageSize);
    
    EagleDbSchema *schema = EagleDbInstance_getSchema(db, EagleDbSchema_DefaultSchemaName);
    
    EagleDbTable *table = EagleDbTable_New("mytable");
    EagleDbTable_addColumn(table, EagleDbColumn_New("col1", EagleDataTypeInteger));
    
    EagleDbTableData *td = EagleDbTableData_New(table, pageSize);
    EagleDbSchema_addTable(schema, td);
    
    for(int i = 0; i < 1; ++i) {
        // create a record
        EagleDbTuple *tuple = EagleDbTuple_New(table);
        EagleDbTuple_setInt(tuple, 0, i);
        
        // put record in
        EagleDbTableData_insert(td, tuple);
        
        EagleDbTuple_Delete(tuple);
    }
    
    /*~ operators ~*/
    OperatorSuite_testOperator(db, "601 >= 417 >= 563", 601 >= 417 >= 563);
    OperatorSuite_testOperator(db, "601 >= 417 <= 695", 601 >= 417 <= 695);
    OperatorSuite_testOperator(db, "601 >= 417 != 479", 601 >= 417 != 479);
    OperatorSuite_testOperator(db, "601 >= 417 > 295", 601 >= 417 > 295);
    OperatorSuite_testOperator(db, "601 >= 417 < 962", 601 >= 417 < 962);
    OperatorSuite_testOperator(db, "601 >= 417 * 432", 601 >= 417 * 432);
    OperatorSuite_testOperator(db, "601 >= 417 + 456", 601 >= 417 + 456);
    OperatorSuite_testOperator(db, "601 >= 417 - 245", 601 >= 417 - 245);
    OperatorSuite_testOperator(db, "601 >= 417 / 837", 601 >= 417 / 837);
    OperatorSuite_testOperator(db, "601 >= 417 % 703", 601 >= 417 % 703);
    OperatorSuite_testOperator(db, "601 >= 417 = 565", 601 >= 417 == 565);
    OperatorSuite_testOperator(db, "601 <= 271 >= 216", 601 <= 271 >= 216);
    OperatorSuite_testOperator(db, "601 <= 271 <= 346", 601 <= 271 <= 346);
    OperatorSuite_testOperator(db, "601 <= 271 != 147", 601 <= 271 != 147);
    OperatorSuite_testOperator(db, "601 <= 271 > 562", 601 <= 271 > 562);
    OperatorSuite_testOperator(db, "601 <= 271 < 436", 601 <= 271 < 436);
    OperatorSuite_testOperator(db, "601 <= 271 * 185", 601 <= 271 * 185);
    OperatorSuite_testOperator(db, "601 <= 271 + 349", 601 <= 271 + 349);
    OperatorSuite_testOperator(db, "601 <= 271 - 326", 601 <= 271 - 326);
    OperatorSuite_testOperator(db, "601 <= 271 / 288", 601 <= 271 / 288);
    OperatorSuite_testOperator(db, "601 <= 271 % 322", 601 <= 271 % 322);
    OperatorSuite_testOperator(db, "601 <= 271 = 599", 601 <= 271 == 599);
    OperatorSuite_testOperator(db, "601 != 226 >= 366", 601 != 226 >= 366);
    OperatorSuite_testOperator(db, "601 != 226 <= 227", 601 != 226 <= 227);
    OperatorSuite_testOperator(db, "601 != 226 != 347", 601 != 226 != 347);
    OperatorSuite_testOperator(db, "601 != 226 > 752", 601 != 226 > 752);
    OperatorSuite_testOperator(db, "601 != 226 < 435", 601 != 226 < 435);
    OperatorSuite_testOperator(db, "601 != 226 * 948", 601 != 226 * 948);
    OperatorSuite_testOperator(db, "601 != 226 + 521", 601 != 226 + 521);
    OperatorSuite_testOperator(db, "601 != 226 - 999", 601 != 226 - 999);
    OperatorSuite_testOperator(db, "601 != 226 / 644", 601 != 226 / 644);
    OperatorSuite_testOperator(db, "601 != 226 % 1", 601 != 226 % 1);
    OperatorSuite_testOperator(db, "601 != 226 = 646", 601 != 226 == 646);
    OperatorSuite_testOperator(db, "601 > 606 >= 433", 601 > 606 >= 433);
    OperatorSuite_testOperator(db, "601 > 606 <= 454", 601 > 606 <= 454);
    OperatorSuite_testOperator(db, "601 > 606 != 204", 601 > 606 != 204);
    OperatorSuite_testOperator(db, "601 > 606 > 622", 601 > 606 > 622);
    OperatorSuite_testOperator(db, "601 > 606 < 157", 601 > 606 < 157);
    OperatorSuite_testOperator(db, "601 > 606 * 769", 601 > 606 * 769);
    OperatorSuite_testOperator(db, "601 > 606 + 246", 601 > 606 + 246);
    OperatorSuite_testOperator(db, "601 > 606 - 725", 601 > 606 - 725);
    OperatorSuite_testOperator(db, "601 > 606 / 115", 601 > 606 / 115);
    OperatorSuite_testOperator(db, "601 > 606 % 745", 601 > 606 % 745);
    OperatorSuite_testOperator(db, "601 > 606 = 639", 601 > 606 == 639);
    OperatorSuite_testOperator(db, "601 < 903 >= 930", 601 < 903 >= 930);
    OperatorSuite_testOperator(db, "601 < 903 <= 989", 601 < 903 <= 989);
    OperatorSuite_testOperator(db, "601 < 903 != 581", 601 < 903 != 581);
    OperatorSuite_testOperator(db, "601 < 903 > 570", 601 < 903 > 570);
    OperatorSuite_testOperator(db, "601 < 903 < 663", 601 < 903 < 663);
    OperatorSuite_testOperator(db, "601 < 903 * 181", 601 < 903 * 181);
    OperatorSuite_testOperator(db, "601 < 903 + 796", 601 < 903 + 796);
    OperatorSuite_testOperator(db, "601 < 903 - 29", 601 < 903 - 29);
    OperatorSuite_testOperator(db, "601 < 903 / 760", 601 < 903 / 760);
    OperatorSuite_testOperator(db, "601 < 903 % 496", 601 < 903 % 496);
    OperatorSuite_testOperator(db, "601 < 903 = 134", 601 < 903 == 134);
    OperatorSuite_testOperator(db, "601 * 196 >= 444", 601 * 196 >= 444);
    OperatorSuite_testOperator(db, "601 * 196 <= 655", 601 * 196 <= 655);
    OperatorSuite_testOperator(db, "601 * 196 != 547", 601 * 196 != 547);
    OperatorSuite_testOperator(db, "601 * 196 > 440", 601 * 196 > 440);
    OperatorSuite_testOperator(db, "601 * 196 < 8", 601 * 196 < 8);
    OperatorSuite_testOperator(db, "601 * 196 * 193", 601 * 196 * 193);
    OperatorSuite_testOperator(db, "601 * 196 + 399", 601 * 196 + 399);
    OperatorSuite_testOperator(db, "601 * 196 - 793", 601 * 196 - 793);
    OperatorSuite_testOperator(db, "601 * 196 / 647", 601 * 196 / 647);
    OperatorSuite_testOperator(db, "601 * 196 % 603", 601 * 196 % 603);
    OperatorSuite_testOperator(db, "601 * 196 = 768", 601 * 196 == 768);
    OperatorSuite_testOperator(db, "601 + 156 >= 372", 601 + 156 >= 372);
    OperatorSuite_testOperator(db, "601 + 156 <= 14", 601 + 156 <= 14);
    OperatorSuite_testOperator(db, "601 + 156 != 233", 601 + 156 != 233);
    OperatorSuite_testOperator(db, "601 + 156 > 839", 601 + 156 > 839);
    OperatorSuite_testOperator(db, "601 + 156 < 111", 601 + 156 < 111);
    OperatorSuite_testOperator(db, "601 + 156 * 873", 601 + 156 * 873);
    OperatorSuite_testOperator(db, "601 + 156 + 94", 601 + 156 + 94);
    OperatorSuite_testOperator(db, "601 + 156 - 41", 601 + 156 - 41);
    OperatorSuite_testOperator(db, "601 + 156 / 214", 601 + 156 / 214);
    OperatorSuite_testOperator(db, "601 + 156 % 675", 601 + 156 % 675);
    OperatorSuite_testOperator(db, "601 + 156 = 612", 601 + 156 == 612);
    OperatorSuite_testOperator(db, "601 - 877 >= 208", 601 - 877 >= 208);
    OperatorSuite_testOperator(db, "601 - 877 <= 760", 601 - 877 <= 760);
    OperatorSuite_testOperator(db, "601 - 877 != 906", 601 - 877 != 906);
    OperatorSuite_testOperator(db, "601 - 877 > 969", 601 - 877 > 969);
    OperatorSuite_testOperator(db, "601 - 877 < 256", 601 - 877 < 256);
    OperatorSuite_testOperator(db, "601 - 877 * 392", 601 - 877 * 392);
    OperatorSuite_testOperator(db, "601 - 877 + 517", 601 - 877 + 517);
    OperatorSuite_testOperator(db, "601 - 877 - 53", 601 - 877 - 53);
    OperatorSuite_testOperator(db, "601 - 877 / 400", 601 - 877 / 400);
    OperatorSuite_testOperator(db, "601 - 877 % 64", 601 - 877 % 64);
    OperatorSuite_testOperator(db, "601 - 877 = 493", 601 - 877 == 493);
    OperatorSuite_testOperator(db, "601 / 408 >= 609", 601 / 408 >= 609);
    OperatorSuite_testOperator(db, "601 / 408 <= 244", 601 / 408 <= 244);
    OperatorSuite_testOperator(db, "601 / 408 != 202", 601 / 408 != 202);
    OperatorSuite_testOperator(db, "601 / 408 > 608", 601 / 408 > 608);
    OperatorSuite_testOperator(db, "601 / 408 < 847", 601 / 408 < 847);
    OperatorSuite_testOperator(db, "601 / 408 * 970", 601 / 408 * 970);
    OperatorSuite_testOperator(db, "601 / 408 + 116", 601 / 408 + 116);
    OperatorSuite_testOperator(db, "601 / 408 - 571", 601 / 408 - 571);
    OperatorSuite_testOperator(db, "601 / 408 / 336", 601 / 408 / 336);
    OperatorSuite_testOperator(db, "601 / 408 % 349", 601 / 408 % 349);
    OperatorSuite_testOperator(db, "601 / 408 = 762", 601 / 408 == 762);
    OperatorSuite_testOperator(db, "601 % 447 >= 222", 601 % 447 >= 222);
    OperatorSuite_testOperator(db, "601 % 447 <= 856", 601 % 447 <= 856);
    OperatorSuite_testOperator(db, "601 % 447 != 840", 601 % 447 != 840);
    OperatorSuite_testOperator(db, "601 % 447 > 788", 601 % 447 > 788);
    OperatorSuite_testOperator(db, "601 % 447 < 884", 601 % 447 < 884);
    OperatorSuite_testOperator(db, "601 % 447 * 804", 601 % 447 * 804);
    OperatorSuite_testOperator(db, "601 % 447 + 665", 601 % 447 + 665);
    OperatorSuite_testOperator(db, "601 % 447 - 444", 601 % 447 - 444);
    OperatorSuite_testOperator(db, "601 % 447 / 917", 601 % 447 / 917);
    OperatorSuite_testOperator(db, "601 % 447 % 924", 601 % 447 % 924);
    OperatorSuite_testOperator(db, "601 % 447 = 765", 601 % 447 == 765);
    OperatorSuite_testOperator(db, "601 = 525 >= 316", 601 == 525 >= 316);
    OperatorSuite_testOperator(db, "601 = 525 <= 282", 601 == 525 <= 282);
    OperatorSuite_testOperator(db, "601 = 525 != 578", 601 == 525 != 578);
    OperatorSuite_testOperator(db, "601 = 525 > 716", 601 == 525 > 716);
    OperatorSuite_testOperator(db, "601 = 525 < 698", 601 == 525 < 698);
    OperatorSuite_testOperator(db, "601 = 525 * 424", 601 == 525 * 424);
    OperatorSuite_testOperator(db, "601 = 525 + 477", 601 == 525 + 477);
    OperatorSuite_testOperator(db, "601 = 525 - 659", 601 == 525 - 659);
    OperatorSuite_testOperator(db, "601 = 525 / 668", 601 == 525 / 668);
    OperatorSuite_testOperator(db, "601 = 525 % 679", 601 == 525 % 679);
    OperatorSuite_testOperator(db, "601 = 525 = 267", 601 == 525 == 267);
    /*~ /operators ~*/
    
    EagleDbTableData_Delete(td);
    EagleDbTable_DeleteWithColumns(table);
    EagleDbInstance_Delete(db);
}

CUNIT_TEST(OperatorSuite, EaglePageOperations_DividePageInt)
{
    int pageSize = 1000;
    EaglePage *page1 = EaglePage_AllocInt(pageSize);
    EaglePage *page2 = EaglePage_AllocInt(pageSize);
    EaglePage *out = EaglePage_AllocInt(pageSize);
    for(int i = 0; i < pageSize; ++i) {
        ((int*) page1->data)[i] = arc4random0(1000);
        ((int*) page2->data)[i] = arc4random0(1000);
    }
    
    // make sure the first one is 0
    ((int*) page2->data)[0] = 0;
    
    EaglePageOperations_DividePageInt(out, page1, page2, NULL);
    
    // modulus by 0
    CUNIT_VERIFY_EQUAL_INT(0, ((int*) out->data)[0]);
    
    int valid = 1;
    for(int i = 1; i < pageSize; ++i) {
        if((((int*) page1->data)[i] / ((int*) page2->data)[i]) != ((int*) out->data)[i]) {
            valid = 0;
            break;
        }
    }
    CUNIT_ASSERT_EQUAL_INT(valid, 1);
    
    EaglePage_Delete(page1);
    EaglePage_Delete(page2);
    EaglePage_Delete(out);
}

CUNIT_TEST(OperatorSuite, EaglePageOperations_NegatePageInt)
{
    int pageSize = 1000;
    EaglePage *page1 = EaglePage_AllocInt(pageSize);
    EaglePage *out = EaglePage_AllocInt(pageSize);
    for(int i = 0; i < pageSize; ++i) {
        ((int*) page1->data)[i] = arc4random() % 100;
    }
    
    EaglePageOperations_NegatePageInt(out, page1, NULL, NULL);
    
    int valid = 1;
    for(int i = 0; i < pageSize; ++i) {
        if(-((int*) page1->data)[i] != ((int*) out->data)[i]) {
            valid = 0;
            break;
        }
    }
    CUNIT_ASSERT_EQUAL_INT(valid, 1);
    
    EaglePage_Delete(page1);
    EaglePage_Delete(out);
}

CUNIT_TEST(OperatorSuite, EaglePageOperations_NotPageInt)
{
    int pageSize = 1000;
    EaglePage *page1 = EaglePage_AllocInt(pageSize);
    EaglePage *out = EaglePage_AllocInt(pageSize);
    for(int i = 0; i < pageSize; ++i) {
        ((int*) page1->data)[i] = arc4random() % 2;
    }
    
    EaglePageOperations_NotPageInt(out, page1, NULL, NULL);
    
    int valid = 1;
    for(int i = 0; i < pageSize; ++i) {
        if((!((int*) page1->data)[i]) != ((int*) out->data)[i]) {
            valid = 0;
            break;
        }
    }
    CUNIT_ASSERT_EQUAL_INT(valid, 1);
    
    EaglePage_Delete(page1);
    EaglePage_Delete(out);
}

CUnitTests* OperatorSuite_tests()
{
    CUnitTests *tests = CUnitTests_New(100);
    
    // method tests
    CUnitTests_addTest(tests, CUNIT_NEW(OperatorSuite, OperatorPrecedence));
    CUnitTests_addTest(tests, CUNIT_NEW_NAME(OperatorSuite, EaglePageOperations_DividePageInt, "page(int) / page(int)"));
    CUnitTests_addTest(tests, CUNIT_NEW_NAME(OperatorSuite, EaglePageOperations_GreaterThanInt, "int > int"));
    CUnitTests_addTest(tests, CUNIT_NEW_NAME(OperatorSuite, EaglePageOperations_LessThanInt, "int < int"));
    CUnitTests_addTest(tests, CUNIT_NEW_NAME(OperatorSuite, EaglePageOperations_GreaterThanPageInt, "page(int) > page(int)"));
    CUnitTests_addTest(tests, CUNIT_NEW_NAME(OperatorSuite, EaglePageOperations_LessThanPageInt, "page(int) < page(int)"));
    CUnitTests_addTest(tests, CUNIT_NEW_NAME(OperatorSuite, EaglePageOperations_GreaterThanEqualPageInt, "page(int) >= page(int)"));
    CUnitTests_addTest(tests, CUNIT_NEW_NAME(OperatorSuite, EaglePageOperations_LessThanEqualPageInt, "page(int) <= page(int)"));
    CUnitTests_addTest(tests, CUNIT_NEW_NAME(OperatorSuite, EaglePageOperations_AndPageInt, "page(int) AND page(int)"));
    CUnitTests_addTest(tests, CUNIT_NEW_NAME(OperatorSuite, EaglePageOperations_AdditionPageInt, "page(int) + page(int)"));
    CUnitTests_addTest(tests, CUNIT_NEW_NAME(OperatorSuite, EaglePageOperations_SubtractPageInt, "page(int) - page(int)"));
    CUnitTests_addTest(tests, CUNIT_NEW_NAME(OperatorSuite, EaglePageOperations_EqualsPageInt, "page(int) = page(int)"));
    CUnitTests_addTest(tests, CUNIT_NEW_NAME(OperatorSuite, EaglePageOperations_NotEqualsPageInt, "page(int) != page(int)"));
    CUnitTests_addTest(tests, CUNIT_NEW_NAME(OperatorSuite, EaglePageOperations_ModulusPageInt, "page(int) % page(int)"));
    CUnitTests_addTest(tests, CUNIT_NEW_NAME(OperatorSuite, EaglePageOperations_MultiplyPageInt, "page(int) * page(int)"));
    CUnitTests_addTest(tests, CUNIT_NEW_NAME(OperatorSuite, EaglePageOperations_NegatePageInt, "- page(int)"));
    CUnitTests_addTest(tests, CUNIT_NEW_NAME(OperatorSuite, EaglePageOperations_NotPageInt, "NOT page(int)"));
    CUnitTests_addTest(tests, CUNIT_NEW_NAME(OperatorSuite, EaglePageOperations_OrPageInt, "page(int) OR page(int)"));
    
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
