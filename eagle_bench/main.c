#include <stdio.h>
#include <stdlib.h>
#include <mach/mach_time.h>
#include "EagleDbTable.h"
#include "EagleDbTableData.h"
#include "EagleDbSqlSelect.h"
#include "EagleInstance.h"
#include "EagleData.h"

extern void *yyparse_ast;
extern int yyerrors_length;
char* yyerrors_last();
void yylex_init();
void yylex_free();
extern int yyparse();
extern int yy_scan_string(const char *);

EagleDbTable* createTable()
{
    /*
     CREATE TABLE mytable (
        col1 INT,
        col2 INT
     );
     */
    EagleDbTable *table = EagleDbTable_New("mytable");
    EagleDbTable_addColumn(table, EagleDbColumn_New("col1", EagleDataTypeInteger));
    return table;
}

EagleDbTableData* fillTable(EagleDbTable *table, int records)
{
    // create the data provider
    EagleDbTableData *td = EagleDbTableData_New(table, 1000);
    
    for(int i = 0; i < records; ++i) {
        // create a record
        EagleDbTuple *tuple = EagleDbTuple_New(table);
        EagleDbTuple_setInt(tuple, 0, i);
        
        // put record in
        EagleDbTableData_insert(td, tuple);
    }
    
    return td;
}

int query(const char *sql)
{
    yylex_init();
    yy_scan_string(sql);
    return yyparse();
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
    uint64_t start = mach_absolute_time();
    int min = 10000000, max = 20000000;
    EaglePageReceiver *receiver = EaglePageReceiver_New();
    EaglePlan *plan = EaglePlan_New(recordsPerPage, receiver);
    
    EaglePageProvider *provider = EaglePageProvider_CreateFromIntArray(data, totalRecords, recordsPerPage);
    
    EaglePlan_addBufferProvider(plan, EaglePlanBufferProvider_New(1, provider));
    
    EaglePlan_addOperation(plan, EaglePlanOperation_New(EaglePageOperations_GreaterThanInt, 2, 1, -1, EagleData_Int(min), EagleTrue,  "1"));
    EaglePlan_addOperation(plan, EaglePlanOperation_New(EaglePageOperations_LessThanInt,    3, 1, -1, EagleData_Int(max), EagleTrue,  "2"));
    EaglePlan_addOperation(plan, EaglePlanOperation_New(EaglePageOperations_AndPage,        0, 2,  3, NULL,               EagleFalse, "3"));
    
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
    
    // clean up
    EagleInstance_Delete(eagle);
    uint64_t duration = mach_absolute_time() - start;
    printf("Done (%.2f seconds)\n", duration / 1000000000.0);
}

int main(int argc, const char * argv[])
{
    int rows = 100000000;
    for(int pageSize = 1000; pageSize <= 100000; pageSize *= 10) {
        printf("%d million rows (%d per page) rows\n", rows / 1000000, pageSize);
        for(int threads = 1; threads < 16; threads *= 2) {
            printf("  %d threads... ", threads);
            
            _instanceTest(threads, pageSize, rows);
        }
    }
    
    /*printf("Creating table with %d (%d per page) rows... ", rows, pageSize);
    start = clock();
    EagleDbTable *table = createTable();
    fillTable(table, rows);
    end = clock();
    printf("Done (%.2f seconds)\n", (float) (end - start) / 1000000.0);
    
    printf("Searching... ");
    start = clock();
    
    if(query("SELECT * FROM mytable WHERE 0")) {
        printf("ERROR: %s\n", yyerrors_last());
        exit(1);
    }
    
    EagleDbSqlSelect *select = (EagleDbSqlSelect*) yyparse_ast;
    
    // compile plan
    EaglePageReceiver *dummy = EaglePageReceiver_New();
    EaglePageProvider *receiver = EaglePageProvider_CreateFromIntStream(pageSize);
    EaglePlan *plan = EaglePlan_New(pageSize, dummy);
    EagleDbSqlExpression_CompilePlanIntoProvider(select->whereExpression, receiver, plan);
    //printf("\n%s\n", EaglePlan_toString(plan));
    
    // execute
    EagleInstance *eagle = EagleInstance_New(1);
    EagleInstance_addPlan(eagle, plan);
    EagleInstance_run(eagle);
    
    // validate result
    printf("Found %d records", receiver->totalRecords);
    
    EaglePageProvider_Delete(receiver);
    EagleInstance_Delete(eagle);
    
    end = clock();
    printf("Done (%.2f seconds)\n", (float) (end - start) / 1000000.0);*/
    
    return 0;
}
