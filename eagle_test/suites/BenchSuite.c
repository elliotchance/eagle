#include "BenchSuite.h"
#include <stdio.h>
#include <stdlib.h>
#include "EagleDbInstance.h"
#include "EagleDbSqlSelect.h"
#include "EagleInstance.h"
#include "EagleDbSchema.h"

void yylex_init();
void yylex_free();
extern int yy_scan_string(const char *);
extern int yyparse();
extern void *yyparse_ast;

static uint64_t BenchSuite_CalibrateAddition = 0;
int BenchSuite_TotalPages = 1000, BenchSuite_RecordsPerPage = 10000;

EagleDbInstance* getInstance()
{
    EagleDbInstance *instance = EagleDbInstance_New(BenchSuite_RecordsPerPage);
    
    // create schema
    EagleDbSchema *schema = EagleDbSchema_New("default");
    EagleDbInstance_addSchema(instance, schema);
    
    // create table
    EagleDbTable *table = EagleDbTable_New("mytable");
    EagleDbTable_addColumn(table, EagleDbColumn_New("col1", EagleDataTypeInteger));
    
    // create table data
    EagleDbTableData *td = EagleDbTableData_New(table, BenchSuite_RecordsPerPage);
    EagleDbSchema_addTable(schema, td);
    
    // tuples
    EagleDbTuple *tuple = EagleDbTuple_New(table);
    for(int i = 1; i <= BenchSuite_TotalPages * BenchSuite_RecordsPerPage; ++i) {
        EagleDbTuple_setInt(tuple, 0, i);
        EagleDbTableData_insert(td, tuple);
    }
    EagleDbTuple_Delete(tuple);
    
    CUNIT_ASSERT_EQUAL_INT(EaglePageProvider_pagesRemaining(td->providers[0]), BenchSuite_TotalPages);
    
    return instance;
}

void CUNIT_ASSERT_BENCH_RESULT(EaglePlan *plan)
{
    double execMultiplier = plan->executionTime / (double) BenchSuite_TotalPages / (double) BenchSuite_RecordsPerPage;
    double executionTime = plan->executionTime / 1.0e9;
    double waitTime = plan->lockWaitTime / 1.0e9;
    //printf(" exec time: %f; wait time: %f; exec multiplier: %f; ", executionTime, waitTime, execMultiplier);
    printf(" exec multiplier: %f; ", execMultiplier);
    
    // make sure the wait time is very small in preportion to the execution time
    CUNIT_VERIFY_LESS_THAN_DOUBLE(waitTime / executionTime, 0.1);
    
    // make sure the execution multiplier is less than 50 - this means that a single record processed should not take
    // any longer than 50x the amount of CPU required to add two numbers
    CUNIT_VERIFY_LESS_THAN_DOUBLE(execMultiplier, 50.0);
}

CUNIT_TEST(BenchSuite, calibrate)
{
    // calculate how long it takes to do a bunch of calculations
    uint64_t start = mach_absolute_time();
    for(int i = 0; i < BenchSuite_TotalPages * BenchSuite_RecordsPerPage; ++i) {
        int a = i;
        a += i;
    }
    uint64_t end = mach_absolute_time();
    BenchSuite_CalibrateAddition = end - start;
    
    printf(" %lld nano seconds; ", BenchSuite_CalibrateAddition);
}

CUNIT_TEST(BenchSuite, SELECT)
{
    EagleDbInstance *instance = getInstance();
    
    // parse SELECT
    yylex_init();
    yy_scan_string("SELECT col1 FROM mytable WHERE col1=1000000");
    yyparse();
    EagleDbSqlSelect *select = (EagleDbSqlSelect*) yyparse_ast;

    // execute
    EaglePlan *plan = EagleDbSqlSelect_parse(select, instance);
    //printf("%s\n", EaglePlan_toString(plan));
    
    // catch compilation error
    CUNIT_ASSERT_EQUAL_INT(EaglePlan_isError(plan), EagleFalse);
    
    // execute
    EagleInstance *eagle = EagleInstance_New(4);
    EagleInstance_addPlan(eagle, plan);
    EagleInstance_run(eagle);
    
    // print results
    CUNIT_ASSERT_EQUAL_INT(EaglePageProvider_pagesRemaining(plan->result[0]), 1);
    EaglePage *p = EaglePageProvider_nextPage(plan->result[0]);
    CUNIT_ASSERT_NOT_NULL(p);
    CUNIT_ASSERT_EQUAL_INT(p->count, 1);
    CUNIT_ASSERT_EQUAL_INT(((int*) p->data)[0], 1000000);
    
    // check timing
    CUNIT_ASSERT_BENCH_RESULT(plan);
    
    EagleInstance_Delete(eagle);
    EagleDbSqlSelect_Delete(select);
}

/**
 * The suite init function.
 */
int BenchSuite_init()
{
    return 0;
}

/**
 * The suite cleanup function.
 */
int BenchSuite_clean()
{
    return 0;
}

CUnitTests* BenchSuite_tests()
{
    CUnitTests *tests = CUnitTests_New(100);
    
    // method tests
    CUnitTests_addTest(tests, CUNIT_NEW(BenchSuite, calibrate));
    CUnitTests_addTest(tests, CUNIT_NEW(BenchSuite, SELECT));
    
    return tests;
}
