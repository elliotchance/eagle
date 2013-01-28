#include "SQLSuite.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "EagleDbTable.h"
#include "EagleDbTuple.h"
#include "EagleDbTableData.h"
#include "EagleInstance.h"
#include "EaglePage.h"
#include "EaglePageProvider.h"
#include "EagleDbSqlSelect.h"
#include "EagleDbInstance.h"
#include "EagleUtils.h"
#include "EagleMemory.h"
#include "EagleDbParser.h"

int _testSqlSelect(const char *sql);

EagleDbTableData **tables;
int allocatedTables = 0;
int usedTables = 0;

typedef struct {
    CUnitTest *test;
    char *sql;
    char *errorMessage;
    EagleDbTuple **answers;
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
        EagleDbTable_DeleteWithColumns(tables[i]->table);
        //EagleDbTableData_Delete(tables[i]);
    }
    
    return 0;
}

void die(char *msg)
{
    perror(msg);
    exit(1);
}

int countColumns(char *line, char ch)
{
    int columnCount = 1;
    for(int i = 0; i < strlen(line); ++i) {
        if(line[i] == ch) {
            ++columnCount;
        }
    }
    return columnCount;
}

char **split(char *line, int *count, char *chs)
{
    *count = countColumns(line, chs[0]);
    
    char **r = (char**) calloc((size_t) *count, sizeof(char*));
    r[0] = strdup(strtok(line, chs));
    for(int i = 1; i < *count; ++i) {
        r[i] = strdup(strtok(NULL, chs));
    }
    
    return r;
}

void SQLSuiteTest()
{
    int exprs = 1;
    int pageSize = 5;
    int i;
    int whereClauseId = -1;
    
    SQLTest test = sqlTests[currentTest++];
    
    // check for parser errors
    if(_testSqlSelect(test.sql)) {
        // expected error
        if(NULL != test.errorMessage) {
            if(!strcmp(EagleDbParser_LastError(), test.errorMessage)) {
                EagleDbParser_Finish();
                return;
            }
            CUNIT_ASSERT_EQUAL_STRING(EagleDbParser_LastError(), test.errorMessage);
        }
        // unexpected error
        else {
            CUNIT_FAIL("%s", EagleDbParser_LastError());
        }
    }
    
    // create the plan skeleton
    EaglePlan *plan = EaglePlan_New(pageSize);
    
    EagleDbParser *p = EagleDbParser_Get();
    EagleDbSqlSelect *select = (EagleDbSqlSelect*) p->yyparse_ast;
    exprs = EagleDbSqlSelect_getExpressionsCount(select);
    EagleDbSqlExpression **expr = (EagleDbSqlExpression**) calloc(exprs, sizeof(EagleDbSqlExpression*));
    for(i = 0; i < EagleDbSqlSelect_getFieldCount(select); ++i) {
        expr[i] = EagleLinkedList_get(select->selectExpressions, i);
    }
    if(NULL != select->whereExpression) {
        whereClauseId = i;
        expr[i] = select->whereExpression;
    }
    
    // get data
    EagleDbTableData *td = tables[0];
    for(int i = 0; i < EagleDbTable_countColumns(td->table); ++i) {
        EaglePageProvider_reset(td->providers[i]);
        EaglePlanBufferProvider *bp = EaglePlanBufferProvider_New(i, td->providers[i], EagleFalse);
        EaglePlan_addBufferProvider(plan, bp, EagleTrue);
    }
    
    // compile plan
    EagleDbSqlExpression_CompilePlan(expr, exprs, whereClauseId, plan);
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
    }
    else {
        // execute
        EagleInstance *eagle = EagleInstance_New(1);
        EagleInstance_addPlan(eagle, plan);
        EagleInstance_run(eagle);
        
        // validate column names
        for(int j = 0; j < EagleDbTable_countColumns(test.answers[0]->table); ++j) {
            CUNIT_ASSERT_EQUAL_STRING(EagleDbTable_getColumn(test.answers[0]->table, j)->name, plan->result[j]->name);
            CUNIT_ASSERT_EQUAL_INT(EagleDbTable_getColumn(test.answers[0]->table, j)->type, plan->result[j]->type);
        }
        
        // validate results
        int valid = 1;
        for(int j = 0; j < EagleDbTable_countColumns(test.answers[0]->table); ++j) {
            EaglePage *page = EaglePageProvider_nextPage(plan->result[j]);
            
            CUNIT_ASSERT_NOT_NULL(page);
            if(NULL != page) {
                CUNIT_ASSERT_EQUAL_INT(page->count, test.usedAnswers);
                CUNIT_ASSERT_EQUAL_INT(page->type, plan->result[j]->type);
                
                for(int i = 0; i < test.usedAnswers; ++i) {
                    switch(page->type) {
                            
                        case EagleDataTypeUnknown:
                            CUNIT_FAIL("%s", "UNKNOWN type");
                            valid = 0;
                            break;
                            
                        case EagleDataTypeInteger:
                            if(*((int*) test.answers[i]->data[j]) != ((int*) page->data)[i]) {
                                CUNIT_FAIL("%d != %d\n", *((int*) test.answers[i]->data[j]), ((int*) page->data)[i]);
                                valid = 0;
                            }
                            break;
                            
                        case EagleDataTypeText:
                            if(strcmp(((char**) test.answers[i]->data)[j], ((char**) page->data)[i])) {
                                CUNIT_FAIL("'%s' != '%s'\n", ((char**) test.answers[i]->data)[j], ((char**) page->data)[i]);
                                valid = 0;
                            }
                            break;
                            
                    }
                    
                    if(valid == 0) {
                        break;
                    }
                }
            }
            
            EaglePage_Delete(page);
        }
        CUNIT_ASSERT_EQUAL_INT(valid, 1);
        
        // clean
        EagleInstance_Delete(eagle);
    }
    
    // clean up
    EaglePlan_Delete(plan);
    EagleMemory_Free(expr);
    
    EagleDbSqlSelect_DeleteRecursive(p->yyparse_ast);
    EagleDbParser_Finish();
}

void controlTest(FILE *file, int *lineNumber)
{
    SQLTest test;
    test.sql = (char*) malloc(1024);
    test.errorMessage = NULL;
    test.test = CUnitTest_New(test.sql, SQLSuiteTest);
    test.usedAnswers = 0;
    test.allocatedAnswers = 5;
    test.answers = (EagleDbTuple**) calloc(test.allocatedAnswers, sizeof(EagleDbTuple*));
    
    // get the SQL
    fgets(test.sql, 1024, file);
    test.sql[strlen(test.sql) - 1] = 0;
    
    // get the definition for the answers
    char line[1024];
    EagleDbTable *table = NULL;
    fgets(line, sizeof(line), file);
    
    int columns = 0;
    char **data = split(strtok(line, "\n"), &columns, ",\n");
    
    table = EagleDbTable_New("result");
    for(int j = 0; j < columns; ++j) {
        int count;
        char **parts = split(data[j], &count, ":");
        EagleDbTable_addColumn(table, EagleDbColumn_New(parts[0], EagleDataType_nameToType(parts[1])));
        EagleMemory_Free(data[j]);
        
        for(int k = 0; k < count; ++k) {
            EagleMemory_Free(parts[k]);
        }
        EagleMemory_Free(parts);
    }
    EagleMemory_Free(data);
    
    // get the answers
    while(fgets(line, sizeof(line), file) != NULL) {
        if(!strcmp("\n", line)) {
            break;
        }
        
        int columns = 0;
        char **data = split(strtok(line, "\n"), &columns, ",\n");
        
        test.answers[test.usedAnswers] = EagleDbTuple_New(table);
        for(int j = 0; j < columns; ++j) {
            switch(EagleDbTable_getColumn(table, j)->type) {
                    
                case EagleDataTypeUnknown:
                    CUNIT_FAIL("%s", "Unknown type");
                    break;
                    
                case EagleDataTypeInteger:
                    EagleDbTuple_setInt(test.answers[test.usedAnswers], j, atoi(data[j]));
                    break;
                    
                case EagleDataTypeText:
                    EagleDbTuple_setText(test.answers[test.usedAnswers], j, data[j]);
                    break;
                    
            }
            EagleMemory_Free(data[j]);
        }
        EagleMemory_Free(data);
        ++test.usedAnswers;
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
    char **columnNames = split(line, &columnCount, ",\n");
    
    // construct the virtual table
    EagleDbTable *table = EagleDbTable_New(tableName);
    for(int i = 0; i < columnCount; ++i) {
        int partsCount;
        char **parts = split(columnNames[i], &partsCount, " ");
        if(partsCount != 2) {
            printf("Wrong number of partsCount for '%s' (got %d)\n", columnNames[i], partsCount);
            exit(1);
        }
        
        EagleDbColumn *column = EagleDbColumn_New(parts[0], EagleDataType_nameToType(parts[1]));
        EagleDbTable_addColumn(table, column);
        
        for(int j = 0; j < partsCount; ++j) {
            EagleMemory_Free(parts[j]);
        }
        EagleMemory_Free(parts);
    }
    EagleDbTableData *td = EagleDbTableData_New(table, 1000);
    tables[usedTables] = td;
    ++usedTables;
    
    // read the data
    while(fgets(line, sizeof(line), file) != NULL) {
        ++*lineNumber;
        
        if(!strcmp("\n", line)) {
            break;
        }
        
        // check we have the right amount of columns
        if(countColumns(line, ',') != columnCount) {
            char msg[1024];
            sprintf(msg, "Expected %d columns on line %d, but found %d columns.", columnCount, *lineNumber, countColumns(line, ','));
            die(msg);
        }
        
        // create the tuple
        int count;
        char **data = split(line, &count, ",\n");
        EagleDbTuple *tuple = EagleDbTuple_New(table);
        for(int i = 0; i < count; ++i) {
            switch(EagleDbTable_getColumn(table, i)->type) {
                    
                case EagleDataTypeUnknown:
                    CUNIT_FAIL("%s", "Unknown type.");
                    break;
                    
                case EagleDataTypeInteger:
                    EagleDbTuple_setInt(tuple, i, atoi(data[i]));
                    break;
                    
                case EagleDataTypeText:
                    EagleDbTuple_setText(tuple, i, data[i]);
                    break;
                    
            }
        }
        
        // insert the data
        EagleDbTableData_insert(td, tuple);
        EagleDbTuple_Delete(tuple);
        
        // clean
        for(int i = 0; i < count; ++i) {
            EagleMemory_Free(data[i]);
        }
        EagleMemory_Free(data);
    }
    
    // clean
    for(int i = 0; i < columnCount; ++i) {
        EagleMemory_Free(columnNames[i]);
    }
    EagleMemory_Free(columnNames);
    EagleMemory_Free(tableName);
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
