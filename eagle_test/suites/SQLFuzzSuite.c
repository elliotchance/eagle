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
    
    // tokens
    "*", // T_ASTERISK
    "+", // T_PLUS
    "=", // T_EQUALS
    ";", // T_END
    ",", // T_COMMA
    "(", // T_BRACKET_OPEN
    ")", // T_BRACKET_CLOSE
    
    // dynamic
    "SomeColumn", // IDENTIFIER
    "1234",       // INTEGER
};
const int totalTokens = sizeof(tokens) / sizeof(char*);

char *getSQLFuzz(char *start, int total)
{
    // build SQL
    char *sql = (char*) malloc(1024);
    sprintf(sql, "%s", start);
    for(int i = 0; i < total; ++i) {
        sprintf(sql, "%s %s", sql, tokens[rand() % totalTokens]);
    }
    
    return sql;
}

CUNIT_TEST(SQLFuzzSuite, All)
{
    // setup
    int pageSize = 10, totalFuzzTests = 10000, length = 5;
    EagleDbInstance *db = EagleDbInstance_New(pageSize);
    
    EagleDbSchema *schema = EagleDbSchema_New((char*) EagleDbSchema_DefaultSchemaName);
    EagleDbInstance_addSchema(db, schema);
    
    EagleDbTable *table = EagleDbTable_New("mytable");
    EagleDbTable_addColumn(table, EagleDbColumn_New("col1", EagleDataTypeInteger));
    
    EagleDbTableData *td = EagleDbTableData_New(table, pageSize);
    EagleDbSchema_addTable(schema, td);
    
    // begin fuzz testing
    srand(0);
    for(int i = 0; i < totalFuzzTests; ++i) {
        char *sql = getSQLFuzz("SELECT", length);
        /*if(i == totalFuzzTests - 1) {
            EagleLogger_Get()->out = stderr;
            fprintf(stderr, "\n%d: %s\n", i, sql);
        }*/
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

CUnitTests* SQLFuzzSuite_tests()
{
    CUnitTests *tests = CUnitTests_New(100);
    
    // method tests
    CUnitTests_addTest(tests, CUNIT_NEW(SQLFuzzSuite, All));
    
    return tests;
}

/**
 * The suite init function.
 */
int SQLFuzzSuite_init()
{
    return 0;
}

/**
 * The suite cleanup function.
 */
int SQLFuzzSuite_clean()
{
    return 0;
}
