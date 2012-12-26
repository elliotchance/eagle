#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "TestSuite.h"
#include "DbSuite.h"
#include "SQLSuite.h"
#include "EagleDbTable.h"
#include "EagleDbTuple.h"
#include "EagleDbTableData.h"
#include "EagleInstance.h"
#include "EaglePage.h"
#include "EaglePageProvider.h"
#include "EagleDbSqlSelect.h"

extern void *yyparse_ast;
void yylex_free();
int _testSqlSelect(const char *sql);
char* yyerrors_last();

EagleDbTableData **tables;
int allocatedTables = 0;
int usedTables = 0;

typedef struct {
    CUnitTest *test;
    char *sql;
    char *errorMessage;
    int *answers;
    int allocatedAnswers;
    int usedAnswers;
} SQLTest;

SQLTest *sqlTests;
int allocatedTests;
int usedTests;
int currentTest;

/**
 * The suite init function.
 */
int SQLSuite_init()
{
    return 0;
}

/**
 * The suite cleanup function.
 */
int SQLSuite_clean()
{
    for(int i = 0; i < usedTables; ++i) {
        EagleDbTable_Delete(tables[i]->table);
        EagleDbTableData_Delete(tables[i]);
    }
    
    return 0;
}

void die(char *msg)
{
    perror(msg);
    exit(1);
}

int countColumns(char *line)
{
    int columnCount = 1;
    for(int i = 0; i < strlen(line); ++i) {
        if(line[i] == ',') {
            ++columnCount;
        }
    }
    return columnCount;
}

char **splitColumns(char *line, int *count)
{
    *count = countColumns(line);
    
    char **r = (char**) calloc((size_t) *count, sizeof(char*));
    r[0] = strdup(strtok(line, ",\n"));
    for(int i = 1; i < *count; ++i) {
        r[i] = strdup(strtok(NULL, ",\n"));
    }
    
    return r;
}

void SQLSuiteTest()
{
    int exprs = 1;
    int pageSize = 5;
    
    SQLTest test = sqlTests[currentTest++];
    
    // check for parser errors
    if(_testSqlSelect(test.sql)) {
        // expected error
        if(NULL != test.errorMessage) {
            if(!strcmp(yyerrors_last(), test.errorMessage)) {
                EagleDbSqlSelect_Delete(yyparse_ast);
                yylex_free();
                return;
            }
            CUNIT_ASSERT_EQUAL_STRING(yyerrors_last(), test.errorMessage);
        }
        // unexpected error
        else {
            CUNIT_FAIL("%s", yyerrors_last());
        }
    }
    EagleDbSqlSelect_Delete(yyparse_ast);
    yylex_free();
    
    EagleDbSqlExpression **expr = (EagleDbSqlExpression**) calloc(exprs, sizeof(EagleDbSqlExpression*));
    expr[0] = _getExpression(test.sql);
    
    // the providers will contain the result
    EaglePageProvider **providers = (EaglePageProvider**) calloc(exprs, sizeof(EaglePageProvider*));
    providers[0] = EaglePageProvider_CreateFromIntStream(pageSize, "answer");
    
    // create the plan skeleton
    EaglePlan *plan = EaglePlan_New(pageSize);
    
    // get data
    EagleDbTableData *td = tables[0];
    for(int i = 0; i < td->table->usedColumns; ++i) {
        EaglePageProvider_reset(td->providers[i]);
        EaglePlanBufferProvider *bp = EaglePlanBufferProvider_New(i, td->providers[i], EagleFalse);
        EaglePlan_addBufferProvider(plan, bp);
    }
    
    // compile plan
    EagleDbSqlExpression_CompilePlan(expr, exprs, -1, providers, plan);
    //printf("%s\n", EaglePlan_toString(plan));
    
    // catch compilation error
    if(EagleTrue == EaglePlan_isError(plan)) {
        // expected error
        if(NULL != test.errorMessage) {
            CUNIT_ASSERT_EQUAL_STRING(plan->errorMessage, test.errorMessage);
        }
        // unexpected error
        else {
            CUNIT_FAIL("%s", plan->errorMessage);
        }
        EaglePlan_Delete(plan);
    }
    else {
        // execute
        EagleInstance *eagle = EagleInstance_New(1);
        EagleInstance_addPlan(eagle, plan);
        EagleInstance_run(eagle);
        
        // validate results
        EaglePage *page = EaglePageProvider_nextPage(providers[0]);
        CUNIT_ASSERT_NOT_NULL(page);
        CUNIT_ASSERT_EQUAL_INT(page->count, test.usedAnswers);
        
        int valid = 1;
        for(int i = 0; i < test.usedAnswers; ++i) {
            //printf("%d != %d\n", test.answers[i], page->data[i]);
            if(test.answers[i] != page->data[i]) {
                valid = 0;
                break;
            }
        }
        CUNIT_ASSERT_EQUAL_INT(valid, 1);
        
        // clean
        EaglePage_Delete(page);
        EagleInstance_Delete(eagle);
    }
    
    // clean up
    for(int i = 0; i < exprs; ++i) {
        EaglePageProvider_Delete(providers[i]);
    }
    free(providers);
    free(expr);
    
    EagleDbSqlSelect_Delete(yyparse_ast);
    yylex_free();
}

void controlTest(FILE *file, int *lineNumber)
{
    SQLTest test;
    test.sql = (char*) malloc(1024);
    test.errorMessage = NULL;
    test.test = CUnitTest_New(test.sql, SQLSuiteTest);
    test.usedAnswers = 0;
    test.allocatedAnswers = 5;
    test.answers = (int*) calloc(test.allocatedAnswers, sizeof(int));
    
    // get the SQL
    fgets(test.sql, 1024, file);
    test.sql[strlen(test.sql) - 1] = 0;
    
    // get the answers
    char line[1024];
    while(fgets(line, sizeof(line), file) != NULL) {
        if(!strcmp("\n", line)) {
            break;
        }
        test.answers[test.usedAnswers++] = atoi(strtok(line, "\n"));
    }
    
    sqlTests[usedTests++] = test;
}

void controlTestError(FILE *file, int *lineNumber)
{
    SQLTest test;
    test.sql = (char*) malloc(1024);
    test.errorMessage = (char*) malloc(1024);
    test.test = CUnitTest_New(test.sql, SQLSuiteTest);
    test.usedAnswers = 0;
    test.allocatedAnswers = 0;
    test.answers = NULL;
    
    // get the SQL
    fgets(test.sql, 1024, file);
    test.sql[strlen(test.sql) - 1] = 0;
    
    // get the expected error message
    fgets(test.errorMessage, 1024, file);
    test.errorMessage[strlen(test.errorMessage) - 1] = 0;
    
    sqlTests[usedTests++] = test;
}

void controlTable(FILE *file, char *firstLine, int *lineNumber)
{
    // get the table name
    char line[1024];
    char *tableName = (char*) malloc(64);
    sscanf(firstLine, "%% table %s", tableName);
    
    // read the column names
    fgets(line, sizeof(line), file);
    ++*lineNumber;
    int columnCount;
    char **columnNames = splitColumns(line, &columnCount);
    
    // construct the virtual table
    EagleDbTable *table = EagleDbTable_New(tableName);
    for(int i = 0; i < columnCount; ++i) {
        EagleDbColumn *column = EagleDbColumn_New(columnNames[i], EagleDbColumnTypeInteger);
        EagleDbTable_addColumn(table, column);
    }
    EagleDbTableData *td = EagleDbTableData_New(table);
    tables[usedTables] = td;
    ++usedTables;
    
    // read the data
    while(fgets(line, sizeof(line), file) != NULL) {
        ++*lineNumber;
        
        if(!strcmp("\n", line)) {
            break;
        }
        
        // check we have the right amount of columns
        if(countColumns(line) != columnCount) {
            char msg[1024];
            sprintf(msg, "Expected %d columns on line %d, but found %d columns.", columnCount, *lineNumber, countColumns(line));
            die(msg);
        }
        
        // create the tuple
        int count;
        char **data = splitColumns(line, &count);
        EagleDbTuple *tuple = EagleDbTuple_New(table);
        for(int i = 0; i < count; ++i) {
            EagleDbTuple_setInt(tuple, i, atoi(data[i]));
        }
        
        // insert the data
        EagleDbTableData_insert(td, tuple);
        EagleDbTuple_Delete(tuple);
        
        // clean
        for(int i = 0; i < count; ++i) {
            free(data[i]);
        }
        free(data);
    }
    
    // clean
    for(int i = 0; i < columnCount; ++i) {
        free(columnNames[i]);
    }
    free(columnNames);
    free(tableName);
}

CUnitTests* SQLSuite_tests()
{
    // init
    allocatedTables = 10;
    usedTables = 0;
    tables = (EagleDbTableData**) calloc(allocatedTables, sizeof(EagleDbTableData*));
    
    allocatedTests = 100;
    usedTests = 0;
    sqlTests = (SQLTest*) calloc(allocatedTests, sizeof(SQLTest));
    
    CUnitTests *tests = CUnitTests_New(allocatedTests);
    
    // open test file
    const char *fileName = "SQLSuite.txt";
    FILE *file = fopen(fileName, "r");
    if(NULL == file) {
        char cwd[1024], msg[1024];
        getcwd(cwd, sizeof(cwd));
        sprintf(msg, "Could not open %s/%s", cwd, fileName);
        die(msg);
    }
    
    // read test file
    char line[1024];
    int lineNumber = 1;
    while(fgets(line, sizeof(line), file) != NULL) {
        /* skip blank lines */
        if(strlen(line) == 0) {
            continue;
        }
        
        /* control line */
        if(line[0] == '%') {
            if(!strncasecmp(line, "% table", strlen("% table"))) {
                controlTable(file, line, &lineNumber);
            }
            else if(!strncasecmp(line, "% test_error", strlen("% test_error"))) {
                controlTestError(file, &lineNumber);
            }
            else if(!strncasecmp(line, "% test", strlen("% test"))) {
                controlTest(file, &lineNumber);
            }
            else {
                char msg[1024];
                sprintf(msg, "Can not understand control line:\n%sError", line);
                die(msg);
            }
        }
    }
    
    for(int i = 0; i < usedTests; ++i) {
        CUnitTests_addTest(tests, sqlTests[i].test);
    }
    
    // clean
    fclose(file);
    
    return tests;
}
