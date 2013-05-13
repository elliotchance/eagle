#include "BenchSuite.h"
#include <stdio.h>
#include <stdlib.h>
#include "EagleDbInstance.h"
#include "EagleDbSqlSelect.h"
#include "EagleInstance.h"
#include "EagleDbSchema.h"
#include "EagleDbColumn.h"
#include "EagleDbParser.h"
#include "EagleUtils.h"

static uint64_t BenchSuite_CalibrateAddition = 0;
int BenchSuite_TotalPages = 1000, BenchSuite_RecordsPerPage = 10000;

void CUNIT_ASSERT_BENCH_RESULT(EaglePlan *plan, int cores)
{
    double executionTime = EaglePlan_getExecutionSeconds(plan);
    double lockTime = EaglePlan_getLockSeconds(plan);
    double realTime = EaglePlan_getRealExecutionSeconds(plan);
    printf("\n real time: %f; CPU time: %f; lock time: %f; ", realTime, executionTime, lockTime);
    //printf(" exec multiplier: %f; ", execMultiplier);
    
    // make sure the wait time is very small in preportion to the execution time
    //CUNIT_VERIFY_LESS_THAN_DOUBLE(waitTime / executionTime, 0.1);
    
    // make sure the execution multiplier is less than 50 - this means that a single record processed should not take
    // any longer than 50x the amount of CPU required to add two numbers
    //CUNIT_VERIFY_LESS_THAN_DOUBLE(execMultiplier, 50.0);
}

CUNIT_TEST(BenchSuite, calibrate)
{
    // calculate how long it takes to do a bunch of calculations
    uint64_t start = EagleUtils_GetAbsoluteTime();
    int a;
    for(int i = 0; i < BenchSuite_TotalPages * BenchSuite_RecordsPerPage; ++i) {
        a = i;
    }
    CUNIT_ASSERT_NOT_NULL(&a);
    uint64_t end = EagleUtils_GetAbsoluteTime();
    BenchSuite_CalibrateAddition = end - start;
    
    printf(" %lld nano seconds; ", BenchSuite_CalibrateAddition);
}

double frand(double max)
{
    return ((double) rand() / (double) RAND_MAX) * max;
}

EagleDbParser* _BenchSuite_distance(EagleDbInstance *db, int rows)
{
    /*
     SLOW INSERT
     for(int i = 0; i < rows; ++i) {
     char sql[1024];
     sprintf(sql, "INSERT INTO point (id, x, y) VALUES (%d, %g, %g);", i + 1, frand(1000.0), frand(1000.0));
     //printf("%s\n", sql);
     
     success = EagleDbInstance_execute(db, sql, &error);
     if(EagleFalse == success) {
     CUNIT_FAIL("%s", error->message);
     }
     }
     */
    
    EagleDbTableData *td = EagleDbInstance_getTable(db, "point");
    EagleDbTuple *tuple = EagleDbTuple_New(td->table);
    for(int i = 0; i < rows; ++i) {
        EagleDbTuple_setInt(tuple, 0, i + 1);
        EagleDbTuple_setFloat(tuple, 1, frand(1000.0));
        EagleDbTuple_setFloat(tuple, 2, frand(1000.0));
        EagleDbTableData_insert(td, tuple);
        
        // this is a bit naughty, to save time on recreating the tuple for every row we are reusing it
        free(tuple->data[0]);
        free(tuple->data[1]);
        free(tuple->data[2]);
        tuple->data[0] = NULL;
        tuple->data[1] = NULL;
        tuple->data[2] = NULL;
    }
    EagleDbTuple_Delete(tuple);
    
    // do a distance search
    EagleDbParser *p = EagleDbParser_ParseWithString("SELECT id, x, y FROM point WHERE sqrt((500.0 - x) * (500.0 - x) + (500.0 - y) * (500.0 - y)) < 1.0");
    if(EagleTrue == EagleDbParser_hasError(p)) {
        CUNIT_FAIL("%s", EagleDbParser_lastError(p));
    }
    
    return p;
}

CUNIT_TEST(BenchSuite, distance)
{
    int pageSize = 1000, rows = 1000000, cores = 8;
    EagleDbInstance *db = EagleDbInstance_New(pageSize, cores);
    EagleLoggerEvent *error = NULL;
    EagleBoolean success;
    
    // create table
    success = EagleDbInstance_execute(db, "CREATE TABLE point (id INT, x DOUBLE, y DOUBLE);", &error);
    if(EagleFalse == success) {
        CUNIT_FAIL("%s", error->message);
    }
    
    // add data
    srand(0);
    EagleDbParser *p = _BenchSuite_distance(db, rows);
        
    // execute
    for(int i = 0; i < 10; ++i) {
        EaglePlan *plan = EagleDbSqlSelect_parse((EagleDbSqlSelect*) p->yyparse_ast, db);
        if(EagleTrue == EaglePlan_isError(plan)) {
            CUNIT_FAIL("%s", plan->errorMessage);
        }
        //printf("%s\n", EaglePlan_toString(plan));
        
        EagleInstance *eagle = EagleInstance_New(cores);
        EagleInstance_addPlan(eagle, plan);
        EagleInstance_run(eagle);
        
        // print results
        /*while(EaglePageProvider_pagesRemaining(plan->result[0]) > 0) {
         EaglePage *id = EaglePageProvider_nextPage(plan->result[0]);
         EaglePage *x = EaglePageProvider_nextPage(plan->result[1]);
         EaglePage *y = EaglePageProvider_nextPage(plan->result[2]);
         
         for(int i = 0; i < id->count; ++i) {
         EagleDataTypeFloatType _x = ((EagleDataTypeFloatType*) x->data)[i];
         EagleDataTypeFloatType _y = ((EagleDataTypeFloatType*) y->data)[i];
         
         printf("(id = %d, x = %g, y = %g) -> %g\n", ((EagleDataTypeIntegerType*) id->data)[i], _x, _y,
         sqrt((500.0 - _x) * (500.0 - _x) + (500.0 - _y) * (500.0 - _y)));
         }
         }*/
        
        // check timing
        CUNIT_ASSERT_BENCH_RESULT(plan, cores);
        EaglePlan_Delete(plan);
    }
    
    EagleDbSqlSelect_DeleteRecursive((EagleDbSqlSelect*) p->yyparse_ast);
    EagleDbParser_Delete(p);
    EagleDbInstance_Delete(db);
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
    CUnitTests_addTest(tests, CUNIT_NEW(BenchSuite, distance));
    
    return tests;
}
