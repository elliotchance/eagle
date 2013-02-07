#include "SQLFuzzSuite.h"
#include <stdio.h>
#include <stdlib.h>
#include "EagleDbInstance.h"
#include "EagleLogger.h"
#include "EagleDbParser.h"

// fuzz tokens
const char *tokens[] = {
    // keywords
    "CREATE",  // K_CREATE
    "FROM",    // K_FROM
    "INT",     // K_INTEGER
    "INTEGER", // K_INTEGER
    "SELECT",  // K_SELECT
    "TABLE",   // K_TABLE
    "TEXT",    // K_TEXT
    "WHERE",   // K_WHERE
    "VALUES",  // K_VALUES
    "INSERT",  // K_INSERT
    "INTO",    // K_INTO
    "AND",     // K_AND
    "OR",      // K_OR
    "NOT",     // K_NOT
    
    // tokens
    ">",  // T_GREATER_THAN
    "<",  // T_LESS_THAN
    ">=", // T_GREATER_THAN_EQUAL
    "<=", // T_LESS_THAN_EQUAL
    "!=", // T_NOT_EQUALS
    "*",  // T_ASTERISK
    "+",  // T_PLUS
    "=",  // T_EQUALS
    ";",  // T_END
    ",",  // T_COMMA
    "(",  // T_BRACKET_OPEN
    ")",  // T_BRACKET_CLOSE
    "-",  // T_MINUS
    "/",  // T_DIVIDE
    "%",  // T_MODULUS
    
    // dynamic
    "SomeColumn", // IDENTIFIER
    "1234",       // INTEGER
};
const int totalTokens = sizeof(tokens) / sizeof(char*);

char *getSQLFuzz(const char *start, int total)
{
    // build SQL
    char *sql = (char*) malloc(1024);
    sprintf(sql, "%s", start);
    for(int i = 0; i < total; ++i) {
        sprintf(sql, "%s %s", sql, tokens[rand() % totalTokens]);
    }
    
    return sql;
}

void runFuzzTests(const char *pre, int totalFuzzTests, int length)
{
    // setup
    int pageSize = 10;
    EagleDbInstance *db = EagleDbInstance_New(pageSize);
    
    EagleDbSchema *schema = EagleDbSchema_New((char*) EagleDbSchema_DefaultSchemaName);
    EagleDbInstance_addSchema(db, schema);
    
    EagleDbTable *table = EagleDbTable_New("mytable");
    EagleDbTable_addColumn(table, EagleDbColumn_New("col1", EagleDataTypeInteger));
    
    EagleDbTableData *td = EagleDbTableData_New(table, pageSize);
    EagleDbSchema_addTable(schema, td);
    
    // begin fuzz testing
    for(int i = 0; i < totalFuzzTests; ++i) {
        char *sql = getSQLFuzz(pre, length);
        
        /*EagleLogger_Get()->out = stderr;
        fprintf(stderr, "\n%d: %s\n", i, sql);*/
        
        EagleBoolean success = EagleDbInstance_execute(db, sql);
        if(EagleTrue == success) {
            CUNIT_FAIL("%s", sql);
        }
        free(sql);
    }
    
    printf(" %d fuzz tests ", totalFuzzTests);
    
    // tear down
    EagleDbTable_DeleteWithColumns(db->schemas[0]->tables[0]->table);
    EagleDbTableData_Delete(db->schemas[0]->tables[0]);
    EagleDbSchema_Delete(db->schemas[0]);
    EagleDbInstance_Delete(db);
}

CUNIT_TEST(SQLFuzzSuite, FirstToken)
{
    EagleDbInstance *db = EagleDbInstance_New(10);
    
    for(int i = 0; i < totalTokens; ++i) {
        if(!strcmp(tokens[i], ";")) {
            continue;
        }
        
        EagleBoolean success = EagleDbInstance_execute(db, tokens[i]);
        if(EagleTrue == success) {
            CUNIT_FAIL("First token \"%s\" did not fail.", tokens[i]);
        }
    }
    
    EagleDbInstance_Delete(db);
}

CUNIT_TEST(SQLFuzzSuite, SELECT)
{
    runFuzzTests("SELECT", 10000, 5);
}

CUNIT_TEST(SQLFuzzSuite, INSERT)
{
    runFuzzTests("INSERT", 10000, 5);
}

CUNIT_TEST(SQLFuzzSuite, CREATE_TABLE)
{
    runFuzzTests("CREATE TABLE", 10000, 5);
}

CUnitTests* SQLFuzzSuite_tests()
{
    CUnitTests *tests = CUnitTests_New(100);
    
    // method tests
    CUnitTests_addTest(tests, CUNIT_NEW(SQLFuzzSuite, FirstToken));
    CUnitTests_addTest(tests, CUNIT_NEW(SQLFuzzSuite, SELECT));
    CUnitTests_addTest(tests, CUNIT_NEW(SQLFuzzSuite, INSERT));
    CUnitTests_addTest(tests, CUNIT_NEW(SQLFuzzSuite, CREATE_TABLE));
    
    return tests;
}

/**
 * The suite init function.
 */
int SQLFuzzSuite_init()
{
    srand(0);
    return 0;
}

/**
 * The suite cleanup function.
 */
int SQLFuzzSuite_clean()
{
    return 0;
}
