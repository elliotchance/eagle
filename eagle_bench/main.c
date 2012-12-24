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
    EagleDbTable_addColumn(table, EagleDbColumn_New("col1", EagleDbColumnTypeInteger));
    
    return table;
}

EagleDbTableData* fillTable(EagleDbTable *table, int records)
{
    // create the data provider
    EagleDbTableData *td = EagleDbTableData_New(table);
    
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

int main(int argc, const char * argv[])
{
    int rows = 1000000, pageSize = 1000;
    size_t start, end;
    
    printf("Creating table with %d (%d per page) rows... ", rows, pageSize);
    start = clock();
    EagleDbTable *table = createTable();
    fillTable(table, rows);
    end = clock();
    printf("Done (%.2f seconds)\n", (float) (end - start) / 1000000.0);
    
    printf("Searching... ");
    start = clock();
    
    if(query("SELECT * FROM mytable WHERE col1 = 1234")) {
        printf("ERROR: %s\n", yyerrors_last());
        exit(1);
    }
    
    EagleDbSqlSelect *select = (EagleDbSqlSelect*) yyparse_ast;
    
    // compile plan
    EaglePageReceiver *dummy = EaglePageReceiver_New();
    EaglePageProvider *receiver = EaglePageProvider_CreateFromIntStream(pageSize, NULL);
    EaglePlan *plan = EaglePlan_New(pageSize, dummy);
    
    // setup the table
    int *col1Data = (int*) calloc((size_t) pageSize, sizeof(int));
    for(int i = 0; i < pageSize; ++i) {
        col1Data[i] = i;
    }
    EaglePageProvider *col1 = EaglePageProvider_CreateFromIntArray(col1Data, pageSize, pageSize, "col1");
    EaglePlan_addBufferProvider(plan, EaglePlanBufferProvider_New(1, col1));
    
    EagleDbSqlExpression_CompilePlanIntoBoolean(select->whereExpression, receiver, plan);
    printf("\n%s\n", EaglePlan_toString(plan));
    
    // execute
    EagleInstance *eagle = EagleInstance_New(1);
    EagleInstance_addPlan(eagle, plan);
    EagleInstance_run(eagle);
    
    // validate result
    EaglePage *result = EaglePageProvider_nextPage(receiver);
    printf("Found %d records", result->count);
    
    EaglePageProvider_Delete(receiver);
    EagleInstance_Delete(eagle);
    
    end = clock();
    printf("Done (%.2f seconds)\n", (float) (end - start) / 1000000.0);
    
    return 0;
}
