#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Eagle.h"
#include "EagleDbParser.h"
#include "EagleDbSqlStatementType.h"
#include "EagleMemory.h"
#include "EagleData.h"
#include "EagleDbSqlExpression.h"
#include "EagleUtils.h"

/**
 Trivial internal type.
 */
typedef void* yyscan_t;

/* internal prototypes supplied by flex and bison */
extern int yylex_destroy(yyscan_t);
extern int yy_scan_string(const char *str, yyscan_t scanner);
extern int yyparse(EagleDbParser*, yyscan_t);

int yylex_init(yyscan_t *ptr_yy_globals);
int yylex_init_extra(EagleDbParser *user_defined, yyscan_t *ptr_yy_globals);
int yylex(yyscan_t yyscanner);
int yylex_destroy(yyscan_t yyscanner);

EagleDbParser* EagleDbParser_New(void)
{
    EagleDbParser *parser = (EagleDbParser*) EagleMemory_Allocate("EagleDbParser_New.1", sizeof(EagleDbParser));
    if(NULL == parser) {
        return NULL;
    }
    
    parser->errors = EagleLinkedList_New();
    parser->yyparse_ast = NULL;
    parser->yystatementtype = EagleDbSqlStatementTypeNone;
    
    return parser;
}

EagleDbParser* EagleDbParser_ParseWithString(const char *sql)
{
    EagleDbParser *parser = EagleDbParser_New();
    
    yylex_init_extra(parser, &parser->yyparse);
    if(NULL != parser) {
        yy_scan_string(sql, parser->yyparse);
        yyparse(parser, parser->yyparse);
    }
    
    return parser;
}

char* EagleDbParser_lastToken(EagleDbParser *p)
{
    return p->yytext_last;
}

EagleBoolean EagleDbParser_hasError(EagleDbParser *p)
{
    return !EagleLinkedList_isEmpty(p->errors);
}

void EagleDbParser_addError(EagleDbParser *p, char *message)
{
    EagleLinkedList_add(p->errors, EagleLinkedListItem_New(strdup(message), EagleTrue, NULL));
}

char* EagleDbParser_lastError(EagleDbParser *p)
{
    if(NULL == p) {
        return NULL;
    }
    if(EagleLinkedList_isEmpty(p->errors)) {
        return NULL;
    }
    return (char*) EagleLinkedList_end(p->errors)->obj;
}

int yyerror(EagleDbParser *parser, void *scanner, char *s)
{
    EagleDbParser_addError(parser, s);
    return 0;
}

void EagleDbParser_Delete(EagleDbParser *p)
{
    if(NULL == p) {
        return;
    }
    
    EagleLinkedList_DeleteWithItems(p->errors);
    if(NULL != p->yyparse) {
        yylex_destroy(p->yyparse);
    }
    EagleMemory_Free(p);
}

EagleBoolean EagleDbParser_IsNonreservedKeyword(const char *word)
{
    unsigned long i;
    static const char *words[] = {
        "A",
        "ABS",
        "ABSOLUTE",
        "ACTION",
        "ADA",
        "ADMIN",
        "AFTER",
        "ALWAYS",
        "ASC",
        "ASSERTION",
        "ASSIGNMENT",
        "ATTRIBUTE",
        "ATTRIBUTES",
        "AVG",
        "BEFORE",
        "BERNOULLI",
        "BREADTH",
        "C",
        "CARDINALITY",
        "CASCADE",
        "CATALOG",
        "CATALOG_NAME",
        "CEIL",
        "CEILING",
        "CHAIN",
        "CHARACTERISTICS",
        "CHARACTERS",
        "CHARACTER_LENGTH",
        "CHARACTER_SET_CATALOG",
        "CHARACTER_SET_NAME",
        "CHARACTER_SET_SCHEMA",
        "CHAR_LENGTH",
        "CHECKED",
        "CLASS_ORIGIN",
        "COALESCE",
        "COBOL",
        "CODE_UNITS",
        "COLLATION",
        "COLLATION_CATALOG",
        "COLLATION_NAME",
        "COLLATION_SCHEMA",
        "COLLECT",
        "COLUMN_NAME",
        "COMMAND_FUNCTION",
        "COMMAND_FUNCTION_CODE",
        "COMMITTED",
        "CONDITION",
        "CONDITION_NUMBER",
        "CONNECTION_NAME",
        "CONSTRAINTS",
        "CONSTRAINT_CATALOG",
        "CONSTRAINT_NAME",
        "CONSTRAINT_SCHEMA",
        "CONSTRUCTORS",
        "CONTAINS",
        "CONVERT",
        "CORR",
        "COUNT",
        "COVAR_POP",
        "COVAR_SAMP",
        "CUME_DIST",
        "CURRENT_COLLATION",
        "CURSOR_NAME",
        "DATA",
        "DATETIME_INTERVAL_CODE",
        "DATETIME_INTERVAL_PRECISION",
        "DEFAULTS",
        "DEFERRABLE",
        "DEFERRED",
        "DEFINED",
        "DEFINER",
        "DEGREE",
        "DENSE_RANK",
        "DEPTH",
        "DERIVED",
        "DESC",
        "DESCRIPTOR",
        "DIAGNOSTICS",
        "DISPATCH",
        "DOMAIN",
        "DYNAMIC_FUNCTION",
        "DYNAMIC_FUNCTION_CODE",
        "EQUALS",
        "EVERY",
        "EXCEPTION",
        "EXCLUDE",
        "EXCLUDING",
        "EXP",
        "EXTRACT",
        "FINAL",
        "FIRST",
        "FLOOR",
        "FOLLOWING",
        "FORTRAN",
        "FOUND",
        "FUSION",
        "G",
        "GENERAL",
        "GO",
        "GOTO",
        "GRANTED",
        "HIERARCHY",
        "IMPLEMENTATION",
        "INCLUDING",
        "INCREMENT",
        "INITIALLY",
        "INSTANCE",
        "INSTANTIABLE",
        "INTERSECTION",
        "INVOKER",
        "K",
        "KEY",
        "KEY_MEMBER",
        "KEY_TYPE",
        "LAST",
        "LENGTH",
        "LEVEL",
        "LN",
        "LOCATOR",
        "LOWER",
        "M",
        "MAP",
        "MATCHED",
        "MAX",
        "MAXVALUE",
        "MESSAGE_LENGTH",
        "MESSAGE_OCTET_LENGTH",
        "MESSAGE_TEXT",
        "MIN",
        "MINVALUE",
        "MOD",
        "MORE",
        "MUMPS",
        "NAME",
        "NAMES",
        "NESTING",
        "NEXT",
        "NORMALIZE",
        "NORMALIZED",
        "NULLABLE",
        "NULLIF",
        "NULLS",
        "NUMBER",
        "OBJECT",
        "OCTETS",
        "OCTET_LENGTH",
        "OPTION",
        "OPTIONS",
        "ORDERING",
        "ORDINALITY",
        "OTHERS",
        "OVERLAY",
        "OVERRIDING",
        "PAD",
        "PARAMETER_MODE",
        "PARAMETER_NAME",
        "PARAMETER_ORDINAL_POSITION",
        "PARAMETER_SPECIFIC_CATALOG",
        "PARAMETER_SPECIFIC_NAME",
        "PARAMETER_SPECIFIC_SCHEMA",
        "PARTIAL",
        "PASCAL",
        "PATH",
        "PERCENTILE_CONT",
        "PERCENTILE_DISC",
        "PERCENT_RANK",
        "PLACING",
        "PLI",
        "POSITION",
        "POWER",
        "PRECEDING",
        "PRESERVE",
        "PRIOR",
        "PRIVILEGES",
        "PUBLIC",
        "RANK",
        "READ",
        "RELATIVE",
        "REPEATABLE",
        "RESTART",
        "RETURNED_CARDINALITY",
        "RETURNED_LENGTH",
        "RETURNED_OCTET_LENGTH",
        "RETURNED_SQLSTATE",
        "ROLE",
        "ROUTINE",
        "ROUTINE_CATALOG",
        "ROUTINE_NAME",
        "ROUTINE_SCHEMA",
        "ROW_COUNT",
        "ROW_NUMBER",
        "SCALE",
        "SCHEMA",
        "SCHEMA_NAME",
        "SCOPE_CATALOG",
        "SCOPE_NAME",
        "SCOPE_SCHEMA",
        "SECTION",
        "SECURITY",
        "SELF",
        "SEQUENCE",
        "SERIALIZABLE",
        "SERVER_NAME",
        "SESSION",
        "SETS",
        "SIMPLE",
        "SIZE",
        "SOURCE",
        "SPACE",
        "SPECIFIC_NAME",
        "SQRT",
        "STATE",
        "STATEMENT",
        "STDDEV_POP",
        "STDDEV_SAMP",
        "STRUCTURE",
        "STYLE",
        "SUBCLASS_ORIGIN",
        "SUBSTRING",
        "SUM",
        "TABLESAMPLE",
        "TABLE_NAME",
        "TEMPORARY",
        "TIES",
        "TOP_LEVEL_COUNT",
        "TRANSACTION",
        "TRANSACTIONS_COMMITTED",
        "TRANSACTIONS_ROLLED_BACK",
        "TRANSACTION_ACTIVE",
        "TRANSFORM",
        "TRANSFORMS",
        "TRANSLATE",
        "TRIGGER_CATALOG",
        "TRIGGER_NAME",
        "TRIGGER_SCHEMA",
        "TRIM",
        "TYPE",
        "UNBOUNDED",
        "UNCOMMITTED",
        "UNDER",
        "UNNAMED",
        "USAGE",
        "USER_DEFINED_TYPE_CATALOG",
        "USER_DEFINED_TYPE_CODE",
        "USER_DEFINED_TYPE_NAME",
        "USER_DEFINED_TYPE_SCHEMA",
        "VIEW",
        "WORK",
        "WRITE",
        "ZONE"
    };
    
    for(i = 0; i < sizeof(words) / sizeof(const char*); ++i) {
        if(EagleTrue == EagleUtils_CompareWithoutCase(word, words[i])) {
            return EagleTrue;
        }
    }
    
    return EagleFalse;
}

EagleBoolean EagleDbParser_IsReservedKeyword(const char *word)
{
    unsigned long i;
    static const char *words[] = {
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
        
        /* reserved words that are not part of the SQL standard */
        "TEXT"
    };
    
    for(i = 0; i < sizeof(words) / sizeof(const char*); ++i) {
        if(EagleTrue == EagleUtils_CompareWithoutCase(word, words[i])) {
            return EagleTrue;
        }
    }
    
    return EagleFalse;
}
