#include "DBSuite.h"
#include <stdio.h>
#include <stdlib.h>
#include "EagleDbSqlSelect.h"
#include "EagleDbSqlBinaryExpression.h"
#include "EagleDbSqlValue.h"
#include "EagleInstance.h"
#include "EagleData.h"
#include "EagleDbColumn.h"
#include "EagleDbTable.h"
#include "EagleDbTableData.h"
#include "EagleDbTuple.h"
#include "EagleDbConsole.h"
#include "EagleMemory.h"
#include "EagleDbParser.h"
#include "EagleLogger.h"
#include "EagleDbInstance.h"
#include "EagleDbSqlUnaryExpression.h"
#include "EaglePageProviderStream.h"
#include "EaglePageProviderArray.h"
#include "EagleDbInformationSchema.h"
#include "EagleDbSqlFunctionExpression.h"

CUNIT_TEST(DBSuite, _INSERT_BadMatch)
{
    EagleDbInstance *db = EagleInstanceTest(10);
    
    EagleLoggerEvent *error = NULL;
    EagleBoolean success = EagleDbInstance_execute(db, "INSERT INTO mytable (col1, col2) VALUES (123);", &error);
    CUNIT_ASSERT_FALSE(success);
    CUNIT_ASSERT_LAST_ERROR("There are 2 columns and 1 values");
    
    EagleInstanceTest_Cleanup(db);
}

CUNIT_TEST(DBSuite, _INSERT_BadColumn1)
{
    EagleDbInstance *db = EagleInstanceTest(10);
    
    EagleLoggerEvent *error = NULL;
    EagleBoolean success = EagleDbInstance_execute(db, "INSERT INTO mytable (123) VALUES (123);", &error);
    CUNIT_ASSERT_FALSE(success);
    CUNIT_ASSERT_LAST_ERROR("You cannot use expressions for column names");
    
    EagleInstanceTest_Cleanup(db);
}

CUNIT_TEST(DBSuite, _INSERT_BadColumn2)
{
    EagleDbInstance *db = EagleInstanceTest(10);
    
    EagleLoggerEvent *error = NULL;
    EagleBoolean success = EagleDbInstance_execute(db, "INSERT INTO mytable (123 + 456) VALUES (123);", &error);
    CUNIT_ASSERT_FALSE(success);
    CUNIT_ASSERT_LAST_ERROR("You cannot use expressions for column names");
    
    EagleInstanceTest_Cleanup(db);
}

CUNIT_TEST(DBSuite, _INSERT_BadValue1)
{
    EagleDbInstance *db = EagleInstanceTest(10);
    
    EagleLoggerEvent *error = NULL;
    EagleBoolean success = EagleDbInstance_execute(db, "INSERT INTO mytable (col1) VALUES (123 + 456);", &error);
    CUNIT_ASSERT_FALSE(success);
    CUNIT_ASSERT_LAST_ERROR("Expressions in VALUES are not yet supported for column 'col1'");
    
    EagleInstanceTest_Cleanup(db);
}

CUNIT_TEST(DBSuite, _INSERT_BadValue2)
{
    EagleDbInstance *db = EagleInstanceTest(10);
    
    EagleLoggerEvent *error = NULL;
    EagleBoolean success = EagleDbInstance_execute(db, "INSERT INTO mytable (col1) VALUES (col1);", &error);
    CUNIT_ASSERT_FALSE(success);
    CUNIT_ASSERT_LAST_ERROR("Only literal values are allowed for expressions.");
    
    EagleInstanceTest_Cleanup(db);
}

CUNIT_TEST(MainSuite, EagleDbSqlSelect_toString)
{
    EagleDbSqlSelect *select = EagleDbSqlSelect_New();
    select->selectExpressions = EagleLinkedList_New();
    EagleLinkedList_addObject(select->selectExpressions, EagleDbSqlValue_NewWithInteger(123), EagleTrue, (void(*)(void*))EagleDbSqlExpression_DeleteRecursive);
    EagleLinkedList_addObject(select->selectExpressions, EagleDbSqlValue_NewWithInteger(456), EagleTrue, (void(*)(void*))EagleDbSqlExpression_DeleteRecursive);
    select->whereExpression = (EagleDbSqlExpression*) EagleDbSqlValue_NewWithInteger(789);
    
    char *desc = EagleDbSqlSelect_toString(select);
    CUNIT_VERIFY_EQUAL_STRING(desc, "SELECT 123, 456 FROM  WHERE 789");
    free(desc);
    
    EagleDbSqlSelect_DeleteRecursive(select);
}

CUNIT_TEST(DBSuite, EagleDbSqlExpression_CompilePlanIntoBuffer_2)
{
    EagleDbSqlExpression *op = (EagleDbSqlExpression*) EagleDbSqlUnaryExpression_New(EagleDbSqlUnaryExpressionOperatorNot, NULL);
    EagleDbSqlExpression *expr = (EagleDbSqlExpression*) EagleDbSqlUnaryExpression_New(EagleDbSqlUnaryExpressionOperatorNot, op);
    EaglePlan *plan = EaglePlan_New(1);
    
    EaglePlan_prepareBuffers(plan, 10);
    int destinationBuffer = 0;
    int result = EagleDbSqlExpression_CompilePlanIntoBuffer_(expr, &destinationBuffer, plan);
    CUNIT_VERIFY_EQUAL_INT(result, EagleDbSqlExpression_ERROR);
    
    EagleDbSqlExpression_DeleteRecursive(expr);
    EaglePlan_Delete(plan);
}

CUNIT_TEST(DBSuite, EagleDbSqlUnaryExpression_Delete)
{
    EagleDbSqlUnaryExpression_Delete(NULL);
}

CUNIT_TEST(DBSuite, EagleDbSqlUnaryExpression_DeleteRecursive)
{
    EagleDbSqlUnaryExpression_DeleteRecursive(NULL);
}

CUNIT_TEST(DBSuite, EagleDbParser_lastError)
{
    CUNIT_VERIFY_NULL(EagleDbParser_lastError(NULL));
    
    EagleDbParser *parser = EagleDbParser_New();
    CUNIT_VERIFY_NULL(EagleDbParser_lastError(parser));
    EagleDbParser_Delete(parser);
}

CUNIT_TEST(DBSuite, EagleDbParser_Delete)
{
    EagleDbParser_Delete(NULL);
}

CUNIT_TEST(DBSuite, EagleDbParser_IsReservedKeyword)
{
    CUNIT_VERIFY_TRUE(EagleDbParser_IsReservedKeyword("CREATE"));
    CUNIT_VERIFY_TRUE(EagleDbParser_IsReservedKeyword("table"));
    CUNIT_VERIFY_FALSE(EagleDbParser_IsReservedKeyword("notakeyword"));
}

CUNIT_TEST(DBSuite, _BadEntityName)
{
    EagleDbInstance *db = EagleInstanceTest(10);
    EagleBoolean success;
    EagleLoggerEvent *error = NULL;
    
    success = EagleDbInstance_execute(db, "CREATE TABLE insert (col1 int);", &error);
    CUNIT_ASSERT_FALSE(success);
    CUNIT_ASSERT_LAST_ERROR("Error: You cannot use the keyword 'INSERT' for an table name.");
    
    success = EagleDbInstance_execute(db, "CREATE TABLE mytable (TABLE int);", &error);
    CUNIT_ASSERT_FALSE(success);
    CUNIT_ASSERT_LAST_ERROR("Error: You cannot use the keyword 'TABLE' for a column name.");
    
    EagleInstanceTest_Cleanup(db);
}

CUNIT_TEST(DBSuite, EagleDbInformationSchema_tables)
{
    EagleDbInstance *db = EagleDbInstance_New(10);
    
    /* parse sql */
    EagleDbParser *p = EagleDbParser_ParseWithString("select table_schema, table_name from information_schema_tables;");
    if(EagleTrue == EagleDbParser_hasError(p)) {
        CUNIT_FAIL("%s", EagleDbParser_lastError(p));
    }
    CUNIT_ASSERT_FALSE(EagleDbParser_hasError(p));
    
    EaglePlan *plan = EagleDbSqlSelect_parse((EagleDbSqlSelect*) p->yyparse_ast, db);
    if(EagleTrue == EaglePlan_isError(plan)) {
        CUNIT_FAIL("%s", plan->errorMessage);
    }
    CUNIT_ASSERT_FALSE(EaglePlan_isError(plan));
    
    /* execute */
    EagleInstance *eagle = EagleInstance_New(1);
    EagleInstance_addPlan(eagle, plan);
    EagleInstance_run(eagle);
    
    /* check results */
    EaglePage *page1 = EaglePageProvider_nextPage(plan->result[0]);
    EaglePage *page2 = EaglePageProvider_nextPage(plan->result[1]);
    CUNIT_ASSERT_NOT_NULL(page1);
    CUNIT_ASSERT_NOT_NULL(page2);
    
    EaglePage_Delete(page1);
    EaglePage_Delete(page2);
    
    EagleDbSqlExpression_DeleteRecursive((EagleDbSqlExpression*) p->yyparse_ast);
    EaglePlan_Delete(plan);
    EagleInstance_Delete(eagle);
    EagleDbParser_Delete(p);
    EagleDbInstance_Delete(db);
}

CUNIT_TEST(DBSuite, EagleDbInformationSchema_Delete)
{
    EagleDbInformationSchema_Delete(NULL);
}

CUNIT_TEST(DBSuite, _comment_multi)
{
    EagleDbInstance *db = EagleInstanceTest(10);
    EagleBoolean success;
    EagleLoggerEvent *error = NULL;
    
    success = EagleDbInstance_execute(db, "CREATE TABLE /* abc */ mytable2 (col1 int);", &error);
    if(EagleFalse == success) {
        CUNIT_FAIL("%s", error->message);
    }
    CUNIT_ASSERT_TRUE(success);
    
    success = EagleDbInstance_execute(db, "CREATE TABLE /* abc", &error);
    CUNIT_ASSERT_FALSE(success);
    CUNIT_ASSERT_LAST_ERROR("Error: syntax error, unexpected end of file");
    
    EagleInstanceTest_Cleanup(db);
}

CUNIT_TEST(DBSuite, _comment_single)
{
    EagleDbInstance *db = EagleInstanceTest(10);
    EagleBoolean success;
    EagleLoggerEvent *error = NULL;
    
    success = EagleDbInstance_execute(db, "-- create a table\nCREATE TABLE mytable2 (col1 int);", &error);
    CUNIT_ASSERT_TRUE(success);
    
    success = EagleDbInstance_execute(db, "CREATE TABLE mytable3 -- create a table\n (col1 int);", &error);
    CUNIT_ASSERT_TRUE(success);
    
    success = EagleDbInstance_execute(db, "CREATE TABLE mytable4 -- create a table (col1 int);", &error);
    CUNIT_ASSERT_FALSE(success);
    CUNIT_ASSERT_LAST_ERROR("Error: syntax error, unexpected end of file, expecting (");
    
    EagleInstanceTest_Cleanup(db);
}

CUNIT_TEST(DBSuite, EagleDbParser_IsNonreservedKeyword)
{
    CUNIT_VERIFY_FALSE(EagleDbParser_IsNonreservedKeyword("CREATE"));
    CUNIT_VERIFY_FALSE(EagleDbParser_IsNonreservedKeyword("table"));
    CUNIT_VERIFY_TRUE(EagleDbParser_IsNonreservedKeyword("A"));
}

CUNIT_TEST(DBSuite, _CREATE)
{
    EagleDbInstance *db = EagleInstanceTest(10);
    EagleBoolean success;
    EagleLoggerEvent *error = NULL;
    
    success = EagleDbInstance_execute(db, "CREATE TABLE mytable2 (col1 int, col2 integer, col3 varchar, col4 text);", &error);
    if(EagleFalse == success) {
        CUNIT_FAIL("%s", error->message);
    }
    CUNIT_ASSERT_TRUE(success);
    
    success = EagleDbInstance_execute(db, "CREATE TABLE mytable2 (col1 int, col2 badtype, col4 text);", &error);
    CUNIT_ASSERT_FALSE(success);
    CUNIT_ASSERT_LAST_ERROR("Error: syntax error, unexpected identifier");
    
    EagleInstanceTest_Cleanup(db);
}

CUNIT_TEST(DBSuite, _string_literal)
{
    EagleDbInstance *db = EagleInstanceTest(10);
    EagleBoolean success;
    EagleLoggerEvent *error = NULL;
    
    success = EagleDbInstance_execute(db, "CREATE TABLE mytable2 (col1 int, col2 text);", &error);
    if(EagleFalse == success) {
        CUNIT_FAIL("%s", error->message);
    }
    CUNIT_ASSERT_TRUE(success);
    
    success = EagleDbInstance_execute(db, "INSERT INTO mytable2 (col1, col2) VALUES (123, 'some '' cool \' text');", &error);
    if(EagleFalse == success) {
        CUNIT_FAIL("%s", error->message);
    }
    CUNIT_ASSERT_TRUE(success);
    
    // SELECT data back
    EagleDbParser *p = EagleDbParser_ParseWithString("SELECT col1, col2, 'some string' FROM mytable2;");
    CUNIT_ASSERT_FALSE(EagleDbParser_hasError(p));
    
    EaglePlan *plan = EagleDbSqlSelect_parse((EagleDbSqlSelect*) p->yyparse_ast, db);
    //printf("%s\n", EaglePlan_toString(plan));
    
    // catch compilation error
    CUNIT_ASSERT_FALSE(EaglePlan_isError(plan));
    
    // execute
    EagleInstance *eagle = EagleInstance_New(1);
    EagleInstance_addPlan(eagle, plan);
    EagleInstance_run(eagle);
    
    // validate data
    EaglePage *page1 = EaglePageProvider_nextPage(plan->result[0]);
    EaglePage *page2 = EaglePageProvider_nextPage(plan->result[1]);
    EaglePage *page3 = EaglePageProvider_nextPage(plan->result[2]);
    
    CUNIT_ASSERT_EQUAL_INT(page1->type, EagleDataTypeInteger);
    CUNIT_ASSERT_EQUAL_INT(page2->type, EagleDataTypeVarchar);
    CUNIT_ASSERT_EQUAL_INT(page3->type, EagleDataTypeVarchar);
    CUNIT_ASSERT_EQUAL_INT(1, page1->count);
    CUNIT_ASSERT_EQUAL_INT(1, page2->count);
    CUNIT_ASSERT_EQUAL_INT(10, page3->count);
    CUNIT_ASSERT_EQUAL_INT(((int*) page1->data)[0], 123);
    CUNIT_ASSERT_EQUAL_STRING(((char**) page2->data)[0], "some ' cool ' text");
    CUNIT_ASSERT_EQUAL_STRING(((char**) page3->data)[0], "some string");
    
    EaglePage_Delete(page1);
    EaglePage_Delete(page2);
    EaglePage_Delete(page3);
    
    EagleInstance_Delete(eagle);
    EaglePlan_Delete(plan);
    
    EagleDbSqlSelect_DeleteRecursive((EagleDbSqlSelect*) p->yyparse_ast);
    EagleDbParser_Delete(p);
    
    EagleInstanceTest_Cleanup(db);
}

CUNIT_TEST(DBSuite, EagleDbSqlValue_toString_2)
{
    EagleDbSqlValue *v = EagleDbSqlValue_NewWithString("some string", EagleFalse);
    char *desc = EagleDbSqlValue_toString(v);
    CUNIT_VERIFY_EQUAL_STRING(desc, "'some string'");
    free(desc);
    EagleDbSqlValue_Delete(v);
}

CUNIT_TEST(DBSuite, EagleDbSqlExpression_CompilePlanIntoBuffer_Function_)
{
    EagleDbSqlFunctionExpression *expr = EagleDbSqlFunctionExpression_New("sqrt", NULL);
    int destinationBuffer;
    EagleDbSqlExpression_CompilePlanIntoBuffer_Function_((EagleDbSqlExpression*) expr, &destinationBuffer, NULL);
    EagleDbSqlExpression_Delete((EagleDbSqlExpression*) expr);
}

CUNIT_TEST(DBSuite, EagleDbSqlFunctionExpression_Delete)
{
    EagleDbSqlFunctionExpression_Delete(NULL);
    
    EagleDbSqlFunctionExpression *expr = EagleDbSqlFunctionExpression_New("sqrt", NULL);
    EagleDbSqlFunctionExpression_Delete(expr);
}

CUNIT_TEST(DBSuite, EagleDbSqlFunctionExpression_DeleteRecursive)
{
    EagleDbSqlFunctionExpression_DeleteRecursive(NULL);
}

CUNIT_TEST(DBSuite, EagleDbSqlBinaryExpression_GetOperation)
{
    EagleDbSqlBinaryOperator op;
    CUNIT_ASSERT_FALSE(EagleDbSqlBinaryExpression_GetOperation(EagleDataTypeUnknown, EagleDbSqlBinaryExpressionOperatorAnd, EagleDataTypeUnknown, &op));
}

CUNIT_TEST(DBSuite, EagleDbSqlValue_toString_3)
{
    {
        EagleDbSqlValue *v = EagleDbSqlValue_NewWithFloat(123.0);
        char *desc = EagleDbSqlValue_toString(v);
        CUNIT_VERIFY_EQUAL_STRING(desc, "123");
        free(desc);
        EagleDbSqlValue_Delete(v);
    }
    
    {
        EagleDbSqlValue *v = EagleDbSqlValue_NewWithFloat(123.456);
        char *desc = EagleDbSqlValue_toString(v);
        CUNIT_VERIFY_EQUAL_STRING(desc, "123.456");
        free(desc);
        EagleDbSqlValue_Delete(v);
    }
}

CUNIT_TEST(DBSuite, EagleDbSqlExpression_CompilePlanIntoBuffer_Binary_)
{
    EagleDbSqlBinaryExpression *expr = EagleDbSqlBinaryExpression_New((EagleDbSqlExpression*) EagleDbSqlValue_NewWithFloat(123.0),
                                                                      EagleDbSqlBinaryExpressionOperatorPlus,
                                                                      (EagleDbSqlExpression*) EagleDbSqlValue_NewWithInteger(456));
    int destinationBuffer = 0;
    
    EaglePlan *plan = EaglePlan_New(1);
    EaglePlan_prepareBuffers(plan, 10);
    
    EagleDbSqlExpression_CompilePlanIntoBuffer_Binary_((EagleDbSqlExpression*) expr, &destinationBuffer, plan);
    CUNIT_ASSERT_EQUAL_STRING(plan->errorMessage, "No such operator FLOAT + INTEGER");
    
    EagleDbSqlExpression_DeleteRecursive((EagleDbSqlExpression*) expr);
    EaglePlan_Delete(plan);
}

CUNIT_TEST(DBSuite, EagleDbTuple_setFloat)
{
    EagleLogger_Get()->out = NULL;
    
    EagleDbTable *table = EagleDbTable_New("mytable");
    EagleDbTable_addColumn(table, EagleDbColumn_New("a", EagleDataTypeVarchar));
    
    EagleDbTuple *tuple = EagleDbTuple_New(table);
    EagleDbTuple_setFloat(tuple, 0, 123.0);
    CUNIT_ASSERT_LAST_ERROR("Wrong type.");
    
    EagleDbTuple_Delete(tuple);
    EagleDbTable_DeleteWithColumns(table);
}

CUnitTests* DBSuite2_tests()
{
    CUnitTests *tests = CUnitTests_New(100);
    
    // method tests
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbTuple_setFloat));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlExpression_CompilePlanIntoBuffer_Binary_));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlValue_toString_3));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlBinaryExpression_GetOperation));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlFunctionExpression_DeleteRecursive));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlFunctionExpression_Delete));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlExpression_CompilePlanIntoBuffer_Function_));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlValue_toString_2));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _string_literal));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _CREATE));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbParser_IsNonreservedKeyword));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _comment_single));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _comment_multi));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbInformationSchema_Delete));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbInformationSchema_tables));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _BadEntityName));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbParser_IsReservedKeyword));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbParser_Delete));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbParser_lastError));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlUnaryExpression_DeleteRecursive));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlUnaryExpression_Delete));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, EagleDbSqlExpression_CompilePlanIntoBuffer_2));
    CUnitTests_addTest(tests, CUNIT_NEW(MainSuite, EagleDbSqlSelect_toString));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _INSERT_BadValue2));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _INSERT_BadValue1));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _INSERT_BadColumn2));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _INSERT_BadColumn1));
    CUnitTests_addTest(tests, CUNIT_NEW(DBSuite, _INSERT_BadMatch));
    
    return tests;
}
