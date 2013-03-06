#include "SQLFuzzSuite.h"
#include <stdio.h>
#include <stdlib.h>
#include "EagleDbInstance.h"
#include "EagleLogger.h"
#include "EagleDbParser.h"

// fuzz tokens
const char *tokens[] = {
    // reserved keywords
    "ADD",
    "ALL",
    "ALLOCATE",
    "ALTER",
    "AND",
    "ANY",
    "ARE",
    "ARRAY",
    "AS",
    "ASENSITIVE",
    "ASYMMETRIC",
    "AT",
    "ATOMIC",
    "AUTHORIZATION",
    "BEGIN",
    "BETWEEN",
    "BIGINT",
    "BINARY",
    "BLOB",
    "BOOLEAN",
    "BOTH",
    "BY",
    "CALL",
    "CALLED",
    "CASCADED",
    "CASE",
    "CAST",
    "CHAR",
    "CHARACTER",
    "CHECK",
    "CLOB",
    "CLOSE",
    "COLLATE",
    "COLUMN",
    "COMMIT",
    "CONNECT",
    "CONSTRAINT",
    "CONTINUE",
    "CORRESPONDING",
    "CREATE",
    "CROSS",
    "CUBE",
    "CURRENT",
    "CURRENT_DATE",
    "CURRENT_DEFAULT_TRANSFORM_GROUP",
    "CURRENT_PATH",
    "CURRENT_ROLE",
    "CURRENT_TIME",
    "CURRENT_TIMESTAMP",
    "CURRENT_TRANSFORM_GROUP_FOR_TYPE",
    "CURRENT_USER",
    "CURSOR",
    "CYCLE",
    "DATE",
    "DAY",
    "DEALLOCATE",
    "DEC",
    "DECIMAL",
    "DECLARE",
    "DEFAULT",
    "DELETE",
    "DEREF",
    "DESCRIBE",
    "DETERMINISTIC",
    "DISCONNECT",
    "DISTINCT",
    "DOUBLE",
    "DROP",
    "DYNAMIC",
    "EACH",
    "ELEMENT",
    "ELSE",
    "END",
    "END-EXEC",
    "ESCAPE",
    "EXCEPT",
    "EXEC",
    "EXECUTE",
    "EXISTS",
    "EXTERNAL",
    "FALSE",
    "FETCH",
    "FILTER",
    "FLOAT",
    "FOR",
    "FOREIGN",
    "FREE",
    "FROM",
    "FULL",
    "FUNCTION",
    "GET",
    "GLOBAL",
    "GRANT",
    "GROUP",
    "GROUPING",
    "HAVING",
    "HOLD",
    "HOUR",
    "IDENTITY",
    "IMMEDIATE",
    "IN",
    "INDICATOR",
    "INNER",
    "INOUT",
    "INPUT",
    "INSENSITIVE",
    "INSERT",
    "INT",
    "INTEGER",
    "INTERSECT",
    "INTERVAL",
    "INTO",
    "IS",
    "ISOLATION",
    "JOIN",
    "LANGUAGE",
    "LARGE",
    "LATERAL",
    "LEADING",
    "LEFT",
    "LIKE",
    "LOCAL",
    "LOCALTIME",
    "LOCALTIMESTAMP",
    "MATCH",
    "MEMBER",
    "MERGE",
    "METHOD",
    "MINUTE",
    "MODIFIES",
    "MODULE",
    "MONTH",
    "MULTISET",
    "NATIONAL",
    "NATURAL",
    "NCHAR",
    "NCLOB",
    "NEW",
    "NO",
    "NONE",
    "NOT",
    "NULL",
    "NUMERIC",
    "OF",
    "OLD",
    "ON",
    "ONLY",
    "OPEN",
    "OR",
    "ORDER",
    "OUT",
    "OUTER",
    "OUTPUT",
    "OVER",
    "OVERLAPS",
    "PARAMETER",
    "PARTITION",
    "PRECISION",
    "PREPARE",
    "PRIMARY",
    "PROCEDURE",
    "RANGE",
    "READS",
    "REAL",
    "RECURSIVE",
    "REF",
    "REFERENCES",
    "REFERENCING",
    "REGR_AVGX",
    "REGR_AVGY",
    "REGR_COUNT",
    "REGR_INTERCEPT",
    "REGR_R2",
    "REGR_SLOPE",
    "REGR_SXX",
    "REGR_SXY",
    "REGR_SYY",
    "RELEASE",
    "RESULT",
    "RETURN",
    "RETURNS",
    "REVOKE",
    "RIGHT",
    "ROLLBACK",
    "ROLLUP",
    "ROW",
    "ROWS",
    "SAVEPOINT",
    "SCROLL",
    "SEARCH",
    "SECOND",
    "SELECT",
    "SENSITIVE",
    "SESSION_USER",
    "SET",
    "SIMILAR",
    "SMALLINT",
    "SOME",
    "SPECIFIC",
    "SPECIFICTYPE",
    "SQL",
    "SQLEXCEPTION",
    "SQLSTATE",
    "SQLWARNING",
    "START",
    "STATIC",
    "SUBMULTISET",
    "SYMMETRIC",
    "SYSTEM",
    "SYSTEM_USER",
    "TABLE",
    "THEN",
    "TIME",
    "TIMESTAMP",
    "TIMEZONE_HOUR",
    "TIMEZONE_MINUTE",
    "TO",
    "TRAILING",
    "TRANSLATION",
    "TREAT",
    "TRIGGER",
    "TRUE",
    "UESCAPE",
    "UNION",
    "UNIQUE",
    "UNKNOWN",
    "UNNEST",
    "UPDATE",
    "UPPER",
    "USER",
    "USING",
    "VALUE",
    "VALUES",
    "VAR_POP",
    "VAR_SAMP",
    "VARCHAR",
    "VARYING",
    "WHEN",
    "WHENEVER",
    "WHERE",
    "WIDTH_BUCKET",
    "WINDOW",
    "WITH",
    "WITHIN",
    "WITHOUT",
    "YEAR",
    
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
    
    EagleDbSchema *schema = EagleDbInstance_getSchema(db, EagleDbSchema_DefaultSchemaName);
    
    EagleDbTable *table = EagleDbTable_New("mytable");
    EagleDbTable_addColumn(table, EagleDbColumn_New("col1", EagleDataTypeInteger));
    
    EagleDbTableData *td = EagleDbTableData_New(table, pageSize);
    EagleDbSchema_addTable(schema, td);
    
    // begin fuzz testing
    for(int i = 0; i < totalFuzzTests; ++i) {
        char *sql = getSQLFuzz(pre, length);
        
        /*EagleLogger_Get()->out = stderr;
        fprintf(stderr, "\n%d: %s\n", i, sql);*/
        
        EagleLoggerEvent *error = NULL;
        EagleBoolean success = EagleDbInstance_execute(db, sql, &error);
        if(EagleTrue == success) {
            CUNIT_FAIL("%s", sql);
        }
        free(sql);
    }
    
    printf(" %d fuzz tests ", totalFuzzTests);
    
    // tear down
    EagleDbSchema *_schema = EagleDbInstance_getSchema(db, EagleDbSchema_DefaultSchemaName);
    EagleDbTableData *_td = EagleDbSchema_getTable(_schema, "mytable");
    EagleDbTable_DeleteWithColumns(_td->table);
    EagleDbTableData_Delete(_td);
    EagleDbInstance_Delete(db);
}

CUNIT_TEST(SQLFuzzSuite, FirstToken)
{
    EagleDbInstance *db = EagleDbInstance_New(10);
    
    for(int i = 0; i < totalTokens; ++i) {
        if(!strcmp(tokens[i], ";")) {
            continue;
        }
        
        EagleLoggerEvent *error = NULL;
        EagleBoolean success = EagleDbInstance_execute(db, tokens[i], &error);
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
