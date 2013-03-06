%{

    #define YYSTYPE void*
    
    typedef struct YYLTYPE
    {
        int first_line;
        int first_column;
        int last_line;
        int last_column;
        char *filename;
    } YYLTYPE;
    
    #include <math.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #include "EagleDbSqlSelect.h"
    #include "EagleDbSqlBinaryExpression.h"
    #include "EagleDbSqlUnaryExpression.h"
    #include "EagleDbTable.h"
    #include "EagleDbColumn.h"
    #include "EagleDbInstance.h"
    #include "EagleMemory.h"
    #include "EagleDbParser.h"
    #include "EagleDataType.h"
    #include "EagleDbSqlInsert.h"
    #include "EagleUtils.h"
    
    int yylex(YYSTYPE *lvalp, YYLTYPE *llocp);
    
    #define ABORT(fmt, ...) { \
    char msg[1024]; \
    sprintf(msg, fmt, __VA_ARGS__); \
    yyerror(parser, NULL, msg); \
    YYERROR; \
    }

%}

/* be more versbose about error messages */
%error-verbose

%pure-parser
%lex-param   { void* scanner }
%parse-param { EagleDbParser *parser }
%parse-param { void* scanner }

/* reserved words */
%token K_ADD "ADD"
%token K_ALL "ALL"
%token K_ALLOCATE "ALLOCATE"
%token K_ALTER "ALTER"
%token K_ANY "ANY"
%token K_ARE "ARE"
%token K_ARRAY "ARRAY"
%token K_AS "AS"
%token K_ASENSITIVE "ASENSITIVE"
%token K_ASYMMETRIC "ASYMMETRIC"
%token K_AT "AT"
%token K_ATOMIC "ATOMIC"
%token K_AUTHORIZATION "AUTHORIZATION"
%token K_BEGIN "BEGIN"
%token K_BETWEEN "BETWEEN"
%token K_BIGINT "BIGINT"
%token K_BINARY "BINARY"
%token K_BLOB "BLOB"
%token K_BOOLEAN "BOOLEAN"
%token K_BOTH "BOTH"
%token K_BY "BY"
%token K_CALL "CALL"
%token K_CALLED "CALLED"
%token K_CASCADED "CASCADED"
%token K_CASE "CASE"
%token K_CAST "CAST"
%token K_CHAR "CHAR"
%token K_CHARACTER "CHARACTER"
%token K_CHECK "CHECK"
%token K_CLOB "CLOB"
%token K_CLOSE "CLOSE"
%token K_COLLATE "COLLATE"
%token K_COLUMN "COLUMN"
%token K_COMMIT "COMMIT"
%token K_CONNECT "CONNECT"
%token K_CONSTRAINT "CONSTRAINT"
%token K_CONTINUE "CONTINUE"
%token K_CORRESPONDING "CORRESPONDING"
%token K_CREATE "CREATE"
%token K_CROSS "CROSS"
%token K_CUBE "CUBE"
%token K_CURRENT "CURRENT"
%token K_CURRENT_DATE "CURRENT_DATE"
%token K_CURRENT_DEFAULT_TRANSFORM_GROUP "CURRENT_DEFAULT_TRANSFORM_GROUP"
%token K_CURRENT_PATH "CURRENT_PATH"
%token K_CURRENT_ROLE "CURRENT_ROLE"
%token K_CURRENT_TIME "CURRENT_TIME"
%token K_CURRENT_TIMESTAMP "CURRENT_TIMESTAMP"
%token K_CURRENT_TRANSFORM_GROUP_FOR_TYPE "CURRENT_TRANSFORM_GROUP_FOR_TYPE"
%token K_CURRENT_USER "CURRENT_USER"
%token K_CURSOR "CURSOR"
%token K_CYCLE "CYCLE"
%token K_DATE "DATE"
%token K_DAY "DAY"
%token K_DEALLOCATE "DEALLOCATE"
%token K_DEC "DEC"
%token K_DECIMAL "DECIMAL"
%token K_DECLARE "DECLARE"
%token K_DEFAULT "DEFAULT"
%token K_DELETE "DELETE"
%token K_DEREF "DEREF"
%token K_DESCRIBE "DESCRIBE"
%token K_DETERMINISTIC "DETERMINISTIC"
%token K_DISCONNECT "DISCONNECT"
%token K_DISTINCT "DISTINCT"
%token K_DOUBLE "DOUBLE"
%token K_DROP "DROP"
%token K_DYNAMIC "DYNAMIC"
%token K_EACH "EACH"
%token K_ELEMENT "ELEMENT"
%token K_ELSE "ELSE"
%token K_END "END"
%token K_END_EXEC "END-EXEC"
%token K_ESCAPE "ESCAPE"
%token K_EXCEPT "EXCEPT"
%token K_EXEC "EXEC"
%token K_EXECUTE "EXECUTE"
%token K_EXISTS "EXISTS"
%token K_EXTERNAL "EXTERNAL"
%token K_FALSE "FALSE"
%token K_FETCH "FETCH"
%token K_FILTER "FILTER"
%token K_FLOAT "FLOAT"
%token K_FOR "FOR"
%token K_FOREIGN "FOREIGN"
%token K_FREE "FREE"
%token K_FROM "FROM"
%token K_FULL "FULL"
%token K_FUNCTION "FUNCTION"
%token K_GET "GET"
%token K_GLOBAL "GLOBAL"
%token K_GRANT "GRANT"
%token K_GROUP "GROUP"
%token K_GROUPING "GROUPING"
%token K_HAVING "HAVING"
%token K_HOLD "HOLD"
%token K_HOUR "HOUR"
%token K_IDENTITY "IDENTITY"
%token K_IMMEDIATE "IMMEDIATE"
%token K_IN "IN"
%token K_INDICATOR "INDICATOR"
%token K_INNER "INNER"
%token K_INOUT "INOUT"
%token K_INPUT "INPUT"
%token K_INSENSITIVE "INSENSITIVE"
%token K_INSERT "INSERT"
%token K_INT "INT"
%token K_INTEGER "INTEGER"
%token K_INTERSECT "INTERSECT"
%token K_INTERVAL "INTERVAL"
%token K_INTO "INTO"
%token K_IS "IS"
%token K_ISOLATION "ISOLATION"
%token K_JOIN "JOIN"
%token K_LANGUAGE "LANGUAGE"
%token K_LARGE "LARGE"
%token K_LATERAL "LATERAL"
%token K_LEADING "LEADING"
%token K_LEFT "LEFT"
%token K_LIKE "LIKE"
%token K_LOCAL "LOCAL"
%token K_LOCALTIME "LOCALTIME"
%token K_LOCALTIMESTAMP "LOCALTIMESTAMP"
%token K_MATCH "MATCH"
%token K_MEMBER "MEMBER"
%token K_MERGE "MERGE"
%token K_METHOD "METHOD"
%token K_MINUTE "MINUTE"
%token K_MODIFIES "MODIFIES"
%token K_MODULE "MODULE"
%token K_MONTH "MONTH"
%token K_MULTISET "MULTISET"
%token K_NATIONAL "NATIONAL"
%token K_NATURAL "NATURAL"
%token K_NCHAR "NCHAR"
%token K_NCLOB "NCLOB"
%token K_NEW "NEW"
%token K_NO "NO"
%token K_NONE "NONE"
%token K_NULL "NULL"
%token K_NUMERIC "NUMERIC"
%token K_OF "OF"
%token K_OLD "OLD"
%token K_ON "ON"
%token K_ONLY "ONLY"
%token K_OPEN "OPEN"
%token K_ORDER "ORDER"
%token K_OUT "OUT"
%token K_OUTER "OUTER"
%token K_OUTPUT "OUTPUT"
%token K_OVER "OVER"
%token K_OVERLAPS "OVERLAPS"
%token K_PARAMETER "PARAMETER"
%token K_PARTITION "PARTITION"
%token K_PRECISION "PRECISION"
%token K_PREPARE "PREPARE"
%token K_PRIMARY "PRIMARY"
%token K_PROCEDURE "PROCEDURE"
%token K_RANGE "RANGE"
%token K_READS "READS"
%token K_REAL "REAL"
%token K_RECURSIVE "RECURSIVE"
%token K_REF "REF"
%token K_REFERENCES "REFERENCES"
%token K_REFERENCING "REFERENCING"
%token K_REGR_AVGX "REGR_AVGX"
%token K_REGR_AVGY "REGR_AVGY"
%token K_REGR_COUNT "REGR_COUNT"
%token K_REGR_INTERCEPT "REGR_INTERCEPT"
%token K_REGR_R2 "REGR_R2"
%token K_REGR_SLOPE "REGR_SLOPE"
%token K_REGR_SXX "REGR_SXX"
%token K_REGR_SXY "REGR_SXY"
%token K_REGR_SYY "REGR_SYY"
%token K_RELEASE "RELEASE"
%token K_RESULT "RESULT"
%token K_RETURN "RETURN"
%token K_RETURNS "RETURNS"
%token K_REVOKE "REVOKE"
%token K_RIGHT "RIGHT"
%token K_ROLLBACK "ROLLBACK"
%token K_ROLLUP "ROLLUP"
%token K_ROW "ROW"
%token K_ROWS "ROWS"
%token K_SAVEPOINT "SAVEPOINT"
%token K_SCROLL "SCROLL"
%token K_SEARCH "SEARCH"
%token K_SECOND "SECOND"
%token K_SELECT "SELECT"
%token K_SENSITIVE "SENSITIVE"
%token K_SESSION_USER "SESSION_USER"
%token K_SET "SET"
%token K_SIMILAR "SIMILAR"
%token K_SMALLINT "SMALLINT"
%token K_SOME "SOME"
%token K_SPECIFIC "SPECIFIC"
%token K_SPECIFICTYPE "SPECIFICTYPE"
%token K_SQL "SQL"
%token K_SQLEXCEPTION "SQLEXCEPTION"
%token K_SQLSTATE "SQLSTATE"
%token K_SQLWARNING "SQLWARNING"
%token K_START "START"
%token K_STATIC "STATIC"
%token K_SUBMULTISET "SUBMULTISET"
%token K_SYMMETRIC "SYMMETRIC"
%token K_SYSTEM "SYSTEM"
%token K_SYSTEM_USER "SYSTEM_USER"
%token K_TABLE "TABLE"
%token K_THEN "THEN"
%token K_TIME "TIME"
%token K_TIMESTAMP "TIMESTAMP"
%token K_TIMEZONE_HOUR "TIMEZONE_HOUR"
%token K_TIMEZONE_MINUTE "TIMEZONE_MINUTE"
%token K_TO "TO"
%token K_TRAILING "TRAILING"
%token K_TRANSLATION "TRANSLATION"
%token K_TREAT "TREAT"
%token K_TRIGGER "TRIGGER"
%token K_TRUE "TRUE"
%token K_UESCAPE "UESCAPE"
%token K_UNION "UNION"
%token K_UNIQUE "UNIQUE"
%token K_UNKNOWN "UNKNOWN"
%token K_UNNEST "UNNEST"
%token K_UPDATE "UPDATE"
%token K_UPPER "UPPER"
%token K_USER "USER"
%token K_USING "USING"
%token K_VALUE "VALUE"
%token K_VALUES "VALUES"
%token K_VAR_POP "VAR_POP"
%token K_VAR_SAMP "VAR_SAMP"
%token K_VARCHAR "VARCHAR"
%token K_VARYING "VARYING"
%token K_WHEN "WHEN"
%token K_WHENEVER "WHENEVER"
%token K_WHERE "WHERE"
%token K_WIDTH_BUCKET "WIDTH_BUCKET"
%token K_WINDOW "WINDOW"
%token K_WITH "WITH"
%token K_WITHIN "WITHIN"
%token K_WITHOUT "WITHOUT"
%token K_YEAR "YEAR"

/* nonreserved words */
%token K_A "A"
%token K_ABS "ABS"
%token K_ABSOLUTE "ABSOLUTE"
%token K_ACTION "ACTION"
%token K_ADA "ADA"
%token K_ADMIN "ADMIN"
%token K_AFTER "AFTER"
%token K_ALWAYS "ALWAYS"
%token K_ASC "ASC"
%token K_ASSERTION "ASSERTION"
%token K_ASSIGNMENT "ASSIGNMENT"
%token K_ATTRIBUTE "ATTRIBUTE"
%token K_ATTRIBUTES "ATTRIBUTES"
%token K_AVG "AVG"
%token K_BEFORE "BEFORE"
%token K_BERNOULLI "BERNOULLI"
%token K_BREADTH "BREADTH"
%token K_C "C"
%token K_CARDINALITY "CARDINALITY"
%token K_CASCADE "CASCADE"
%token K_CATALOG "CATALOG"
%token K_CATALOG_NAME "CATALOG_NAME"
%token K_CEIL "CEIL"
%token K_CEILING "CEILING"
%token K_CHAIN "CHAIN"
%token K_CHARACTERISTICS "CHARACTERISTICS"
%token K_CHARACTERS "CHARACTERS"
%token K_CHARACTER_LENGTH "CHARACTER_LENGTH"
%token K_CHARACTER_SET_CATALOG "CHARACTER_SET_CATALOG"
%token K_CHARACTER_SET_NAME "CHARACTER_SET_NAME"
%token K_CHARACTER_SET_SCHEMA "CHARACTER_SET_SCHEMA"
%token K_CHAR_LENGTH "CHAR_LENGTH"
%token K_CHECKED "CHECKED"
%token K_CLASS_ORIGIN "CLASS_ORIGIN"
%token K_COALESCE "COALESCE"
%token K_COBOL "COBOL"
%token K_CODE_UNITS "CODE_UNITS"
%token K_COLLATION "COLLATION"
%token K_COLLATION_CATALOG "COLLATION_CATALOG"
%token K_COLLATION_NAME "COLLATION_NAME"
%token K_COLLATION_SCHEMA "COLLATION_SCHEMA"
%token K_COLLECT "COLLECT"
%token K_COLUMN_NAME "COLUMN_NAME"
%token K_COMMAND_FUNCTION "COMMAND_FUNCTION"
%token K_COMMAND_FUNCTION_CODE "COMMAND_FUNCTION_CODE"
%token K_COMMITTED "COMMITTED"
%token K_CONDITION "CONDITION"
%token K_CONDITION_NUMBER "CONDITION_NUMBER"
%token K_CONNECTION_NAME "CONNECTION_NAME"
%token K_CONSTRAINTS "CONSTRAINTS"
%token K_CONSTRAINT_CATALOG "CONSTRAINT_CATALOG"
%token K_CONSTRAINT_NAME "CONSTRAINT_NAME"
%token K_CONSTRAINT_SCHEMA "CONSTRAINT_SCHEMA"
%token K_CONSTRUCTORS "CONSTRUCTORS"
%token K_CONTAINS "CONTAINS"
%token K_CONVERT "CONVERT"
%token K_CORR "CORR"
%token K_COUNT "COUNT"
%token K_COVAR_POP "COVAR_POP"
%token K_COVAR_SAMP "COVAR_SAMP"
%token K_CUME_DIST "CUME_DIST"
%token K_CURRENT_COLLATION "CURRENT_COLLATION"
%token K_CURSOR_NAME "CURSOR_NAME"
%token K_DATA "DATA"
%token K_DATETIME_INTERVAL_CODE "DATETIME_INTERVAL_CODE"
%token K_DATETIME_INTERVAL_PRECISION "DATETIME_INTERVAL_PRECISION"
%token K_DEFAULTS "DEFAULTS"
%token K_DEFERRABLE "DEFERRABLE"
%token K_DEFERRED "DEFERRED"
%token K_DEFINED "DEFINED"
%token K_DEFINER "DEFINER"
%token K_DEGREE "DEGREE"
%token K_DENSE_RANK "DENSE_RANK"
%token K_DEPTH "DEPTH"
%token K_DERIVED "DERIVED"
%token K_DESC "DESC"
%token K_DESCRIPTOR "DESCRIPTOR"
%token K_DIAGNOSTICS "DIAGNOSTICS"
%token K_DISPATCH "DISPATCH"
%token K_DOMAIN "DOMAIN"
%token K_DYNAMIC_FUNCTION "DYNAMIC_FUNCTION"
%token K_DYNAMIC_FUNCTION_CODE "DYNAMIC_FUNCTION_CODE"
%token K_EQUALS "EQUALS"
%token K_EVERY "EVERY"
%token K_EXCEPTION "EXCEPTION"
%token K_EXCLUDE "EXCLUDE"
%token K_EXCLUDING "EXCLUDING"
%token K_EXP "EXP"
%token K_EXTRACT "EXTRACT"
%token K_FINAL "FINAL"
%token K_FIRST "FIRST"
%token K_FLOOR "FLOOR"
%token K_FOLLOWING "FOLLOWING"
%token K_FORTRAN "FORTRAN"
%token K_FOUND "FOUND"
%token K_FUSION "FUSION"
%token K_G "G"
%token K_GENERAL "GENERAL"
%token K_GO "GO"
%token K_GOTO "GOTO"
%token K_GRANTED "GRANTED"
%token K_HIERARCHY "HIERARCHY"
%token K_IMPLEMENTATION "IMPLEMENTATION"
%token K_INCLUDING "INCLUDING"
%token K_INCREMENT "INCREMENT"
%token K_INITIALLY "INITIALLY"
%token K_INSTANCE "INSTANCE"
%token K_INSTANTIABLE "INSTANTIABLE"
%token K_INTERSECTION "INTERSECTION"
%token K_INVOKER "INVOKER"
%token K_K "K"
%token K_KEY "KEY"
%token K_KEY_MEMBER "KEY_MEMBER"
%token K_KEY_TYPE "KEY_TYPE"
%token K_LAST "LAST"
%token K_LENGTH "LENGTH"
%token K_LEVEL "LEVEL"
%token K_LN "LN"
%token K_LOCATOR "LOCATOR"
%token K_LOWER "LOWER"
%token K_M "M"
%token K_MAP "MAP"
%token K_MATCHED "MATCHED"
%token K_MAX "MAX"
%token K_MAXVALUE "MAXVALUE"
%token K_MESSAGE_LENGTH "MESSAGE_LENGTH"
%token K_MESSAGE_OCTET_LENGTH "MESSAGE_OCTET_LENGTH"
%token K_MESSAGE_TEXT "MESSAGE_TEXT"
%token K_MIN "MIN"
%token K_MINVALUE "MINVALUE"
%token K_MOD "MOD"
%token K_MORE "MORE"
%token K_MUMPS "MUMPS"
%token K_NAME "NAME"
%token K_NAMES "NAMES"
%token K_NESTING "NESTING"
%token K_NEXT "NEXT"
%token K_NORMALIZE "NORMALIZE"
%token K_NORMALIZED "NORMALIZED"
%token K_NULLABLE "NULLABLE"
%token K_NULLIF "NULLIF"
%token K_NULLS "NULLS"
%token K_NUMBER "NUMBER"
%token K_OBJECT "OBJECT"
%token K_OCTETS "OCTETS"
%token K_OCTET_LENGTH "OCTET_LENGTH"
%token K_OPTION "OPTION"
%token K_OPTIONS "OPTIONS"
%token K_ORDERING "ORDERING"
%token K_ORDINALITY "ORDINALITY"
%token K_OTHERS "OTHERS"
%token K_OVERLAY "OVERLAY"
%token K_OVERRIDING "OVERRIDING"
%token K_PAD "PAD"
%token K_PARAMETER_MODE "PARAMETER_MODE"
%token K_PARAMETER_NAME "PARAMETER_NAME"
%token K_PARAMETER_ORDINAL_POSITION "PARAMETER_ORDINAL_POSITION"
%token K_PARAMETER_SPECIFIC_CATALOG "PARAMETER_SPECIFIC_CATALOG"
%token K_PARAMETER_SPECIFIC_NAME "PARAMETER_SPECIFIC_NAME"
%token K_PARAMETER_SPECIFIC_SCHEMA "PARAMETER_SPECIFIC_SCHEMA"
%token K_PARTIAL "PARTIAL"
%token K_PASCAL "PASCAL"
%token K_PATH "PATH"
%token K_PERCENTILE_CONT "PERCENTILE_CONT"
%token K_PERCENTILE_DISC "PERCENTILE_DISC"
%token K_PERCENT_RANK "PERCENT_RANK"
%token K_PLACING "PLACING"
%token K_PLI "PLI"
%token K_POSITION "POSITION"
%token K_POWER "POWER"
%token K_PRECEDING "PRECEDING"
%token K_PRESERVE "PRESERVE"
%token K_PRIOR "PRIOR"
%token K_PRIVILEGES "PRIVILEGES"
%token K_PUBLIC "PUBLIC"
%token K_RANK "RANK"
%token K_READ "READ"
%token K_RELATIVE "RELATIVE"
%token K_REPEATABLE "REPEATABLE"
%token K_RESTART "RESTART"
%token K_RETURNED_CARDINALITY "RETURNED_CARDINALITY"
%token K_RETURNED_LENGTH "RETURNED_LENGTH"
%token K_RETURNED_OCTET_LENGTH "RETURNED_OCTET_LENGTH"
%token K_RETURNED_SQLSTATE "RETURNED_SQLSTATE"
%token K_ROLE "ROLE"
%token K_ROUTINE "ROUTINE"
%token K_ROUTINE_CATALOG "ROUTINE_CATALOG"
%token K_ROUTINE_NAME "ROUTINE_NAME"
%token K_ROUTINE_SCHEMA "ROUTINE_SCHEMA"
%token K_ROW_COUNT "ROW_COUNT"
%token K_ROW_NUMBER "ROW_NUMBER"
%token K_SCALE "SCALE"
%token K_SCHEMA "SCHEMA"
%token K_SCHEMA_NAME "SCHEMA_NAME"
%token K_SCOPE_CATALOG "SCOPE_CATALOG"
%token K_SCOPE_NAME "SCOPE_NAME"
%token K_SCOPE_SCHEMA "SCOPE_SCHEMA"
%token K_SECTION "SECTION"
%token K_SECURITY "SECURITY"
%token K_SELF "SELF"
%token K_SEQUENCE "SEQUENCE"
%token K_SERIALIZABLE "SERIALIZABLE"
%token K_SERVER_NAME "SERVER_NAME"
%token K_SESSION "SESSION"
%token K_SETS "SETS"
%token K_SIMPLE "SIMPLE"
%token K_SIZE "SIZE"
%token K_SOURCE "SOURCE"
%token K_SPACE "SPACE"
%token K_SPECIFIC_NAME "SPECIFIC_NAME"
%token K_SQRT "SQRT"
%token K_STATE "STATE"
%token K_STATEMENT "STATEMENT"
%token K_STDDEV_POP "STDDEV_POP"
%token K_STDDEV_SAMP "STDDEV_SAMP"
%token K_STRUCTURE "STRUCTURE"
%token K_STYLE "STYLE"
%token K_SUBCLASS_ORIGIN "SUBCLASS_ORIGIN"
%token K_SUBSTRING "SUBSTRING"
%token K_SUM "SUM"
%token K_TABLESAMPLE "TABLESAMPLE"
%token K_TABLE_NAME "TABLE_NAME"
%token K_TEMPORARY "TEMPORARY"
%token K_TIES "TIES"
%token K_TOP_LEVEL_COUNT "TOP_LEVEL_COUNT"
%token K_TRANSACTION "TRANSACTION"
%token K_TRANSACTIONS_COMMITTED "TRANSACTIONS_COMMITTED"
%token K_TRANSACTIONS_ROLLED_BACK "TRANSACTIONS_ROLLED_BACK"
%token K_TRANSACTION_ACTIVE "TRANSACTION_ACTIVE"
%token K_TRANSFORM "TRANSFORM"
%token K_TRANSFORMS "TRANSFORMS"
%token K_TRANSLATE "TRANSLATE"
%token K_TRIGGER_CATALOG "TRIGGER_CATALOG"
%token K_TRIGGER_NAME "TRIGGER_NAME"
%token K_TRIGGER_SCHEMA "TRIGGER_SCHEMA"
%token K_TRIM "TRIM"
%token K_TYPE "TYPE"
%token K_UNBOUNDED "UNBOUNDED"
%token K_UNCOMMITTED "UNCOMMITTED"
%token K_UNDER "UNDER"
%token K_UNNAMED "UNNAMED"
%token K_USAGE "USAGE"
%token K_USER_DEFINED_TYPE_CATALOG "USER_DEFINED_TYPE_CATALOG"
%token K_USER_DEFINED_TYPE_CODE "USER_DEFINED_TYPE_CODE"
%token K_USER_DEFINED_TYPE_NAME "USER_DEFINED_TYPE_NAME"
%token K_USER_DEFINED_TYPE_SCHEMA "USER_DEFINED_TYPE_SCHEMA"
%token K_VIEW "VIEW"
%token K_WORK "WORK"
%token K_WRITE "WRITE"
%token K_ZONE "ZONE"

%token K_TEXT "TEXT"

/* variable tokens */
%token IDENTIFIER "identifier"
%token INTEGER "integer"
%token STRING_LITERAL "string_literal"

/* fixed tokens */
%token T_END           ";"
%token T_COMMA         ","
%token T_BRACKET_OPEN  "("
%token T_BRACKET_CLOSE ")"

/* operators */
%left  K_OR             "OR"
%left  K_AND           "AND"
%left  T_NOT_EQUALS     "!="  T_EQUALS    "="
%left  T_GREATER_THAN    ">"  T_LESS_THAN "<"  T_GREATER_THAN_EQUAL ">="  T_LESS_THAN_EQUAL "<="
%left  T_PLUS            "+"  T_MINUS     "-"
%left  K_NOT           "NOT"
%left  T_ASTERISK        "*"  T_DIVIDE    "/"  T_MODULUS             "%"

%token END 0 "end of file"

/*%type <parser> statement*/

%destructor { EagleLinkedList_DeleteWithItems($$); } column_expression_list column_definition_list
%destructor { EagleDbSqlValue_Delete($$); } integer identifier value
%destructor { EagleDbSqlExpression_DeleteRecursive($$); } column_expression expression where_expression
%destructor { EagleDbSqlSelect_Delete($$); } select_statement
%destructor { EagleMemory_Free($$); } data_type
%destructor { EagleDbColumn_Delete($$); } column_definition
%destructor { EagleDbTable_Delete($$); } create_table_statement
%destructor { EagleDbSqlInsert_Delete($$); } insert_statement

%%

input:
    END
    |
    T_END END
    |
    statement END {
        parser->yyparse_ast = $1;
    }
    |
    statement T_END END {
        parser->yyparse_ast = $1;
    }
;

statement:
      select_statement { parser->yystatementtype = EagleDbSqlStatementTypeSelect; }
    | create_table_statement { parser->yystatementtype = EagleDbSqlStatementTypeCreateTable; }
    | insert_statement { parser->yystatementtype = EagleDbSqlStatementTypeInsert; }
;

insert_statement:
    K_INSERT K_INTO identifier
    T_BRACKET_OPEN column_expression_list T_BRACKET_CLOSE
    K_VALUES T_BRACKET_OPEN column_expression_list T_BRACKET_CLOSE
    {
        EagleDbSqlInsert *insert = EagleDbSqlInsert_New();
        char *table = strdup(((EagleDbSqlValue*) $3)->value.identifier);
        EagleDbSqlValue_Delete($3);
        insert->table = table;
        insert->names = $5;
        insert->values = $9;
        $$ = insert;
    }
;

create_table_statement:
    K_CREATE K_TABLE keyword error
    {
        ABORT("You cannot use the keyword '%s' for an table name.", $3);
    }
    |
    K_CREATE K_TABLE identifier T_BRACKET_OPEN column_definition_list T_BRACKET_CLOSE
    {
        EagleDbTable *table = EagleDbTable_New(((EagleDbSqlValue*) $3)->value.identifier);
        EagleDbTable_setColumns(table, $5);
        EagleDbSqlValue_Delete($3);
        $$ = table;
    }
;

column_definition_list:
    column_definition {
        $$ = EagleLinkedList_New();
        EagleLinkedListItem *item = EagleLinkedListItem_New($1, EagleTrue, (void(*)(void*)) EagleDbColumn_Delete);
        EagleLinkedList_add($$, item);
    }
    |
    column_definition_list T_COMMA column_definition {
        EagleLinkedListItem *item = EagleLinkedListItem_New($3, EagleTrue, (void(*)(void*)) EagleDbColumn_Delete);
        EagleLinkedList_add($1, item);
    }
;

column_definition:
    keyword error {
        ABORT("You cannot use the keyword '%s' for a column name.", $1);
    }
    |
    identifier data_type {
        $$ = EagleDbColumn_New(((EagleDbSqlValue*) $1)->value.identifier, *((int*) $2));
        EagleDbSqlValue_Delete($1);
        EagleMemory_Free($2);
    }
;

data_type:
      K_INTEGER { $$ = EagleData_Int(EagleDataTypeInteger); }
    | K_INT     { $$ = EagleData_Int(EagleDataTypeInteger); }
    | K_VARCHAR { $$ = EagleData_Int(EagleDataTypeVarchar); }
    | K_TEXT    { $$ = EagleData_Int(EagleDataTypeVarchar); }
;

keyword:
    nonreserved_word | reserved_word
;

nonreserved_word:
      K_A { $$ = "A"; }
    | K_ABS { $$ = "ABS"; }
    | K_ABSOLUTE { $$ = "ABSOLUTE"; }
    | K_ACTION { $$ = "ACTION"; }
    | K_ADA { $$ = "ADA"; }
    | K_ADMIN { $$ = "ADMIN"; }
    | K_AFTER { $$ = "AFTER"; }
    | K_ALWAYS { $$ = "ALWAYS"; }
    | K_ASC { $$ = "ASC"; }
    | K_ASSERTION { $$ = "ASSERTION"; }
    | K_ASSIGNMENT { $$ = "ASSIGNMENT"; }
    | K_ATTRIBUTE { $$ = "ATTRIBUTE"; }
    | K_ATTRIBUTES { $$ = "ATTRIBUTES"; }
    | K_AVG { $$ = "AVG"; }
    | K_BEFORE { $$ = "BEFORE"; }
    | K_BERNOULLI { $$ = "BERNOULLI"; }
    | K_BREADTH { $$ = "BREADTH"; }
    | K_C { $$ = "C"; }
    | K_CARDINALITY { $$ = "CARDINALITY"; }
    | K_CASCADE { $$ = "CASCADE"; }
    | K_CATALOG { $$ = "CATALOG"; }
    | K_CATALOG_NAME { $$ = "CATALOG_NAME"; }
    | K_CEIL { $$ = "CEIL"; }
    | K_CEILING { $$ = "CEILING"; }
    | K_CHAIN { $$ = "CHAIN"; }
    | K_CHARACTERISTICS { $$ = "CHARACTERISTICS"; }
    | K_CHARACTERS { $$ = "CHARACTERS"; }
    | K_CHARACTER_LENGTH { $$ = "CHARACTER_LENGTH"; }
    | K_CHARACTER_SET_CATALOG { $$ = "CHARACTER_SET_CATALOG"; }
    | K_CHARACTER_SET_NAME { $$ = "CHARACTER_SET_NAME"; }
    | K_CHARACTER_SET_SCHEMA { $$ = "CHARACTER_SET_SCHEMA"; }
    | K_CHAR_LENGTH { $$ = "CHAR_LENGTH"; }
    | K_CHECKED { $$ = "CHECKED"; }
    | K_CLASS_ORIGIN { $$ = "CLASS_ORIGIN"; }
    | K_COALESCE { $$ = "COALESCE"; }
    | K_COBOL { $$ = "COBOL"; }
    | K_CODE_UNITS { $$ = "CODE_UNITS"; }
    | K_COLLATION { $$ = "COLLATION"; }
    | K_COLLATION_CATALOG { $$ = "COLLATION_CATALOG"; }
    | K_COLLATION_NAME { $$ = "COLLATION_NAME"; }
    | K_COLLATION_SCHEMA { $$ = "COLLATION_SCHEMA"; }
    | K_COLLECT { $$ = "COLLECT"; }
    | K_COLUMN_NAME { $$ = "COLUMN_NAME"; }
    | K_COMMAND_FUNCTION { $$ = "COMMAND_FUNCTION"; }
    | K_COMMAND_FUNCTION_CODE { $$ = "COMMAND_FUNCTION_CODE"; }
    | K_COMMITTED { $$ = "COMMITTED"; }
    | K_CONDITION { $$ = "CONDITION"; }
    | K_CONDITION_NUMBER { $$ = "CONDITION_NUMBER"; }
    | K_CONNECTION_NAME { $$ = "CONNECTION_NAME"; }
    | K_CONSTRAINTS { $$ = "CONSTRAINTS"; }
    | K_CONSTRAINT_CATALOG { $$ = "CONSTRAINT_CATALOG"; }
    | K_CONSTRAINT_NAME { $$ = "CONSTRAINT_NAME"; }
    | K_CONSTRAINT_SCHEMA { $$ = "CONSTRAINT_SCHEMA"; }
    | K_CONSTRUCTORS { $$ = "CONSTRUCTORS"; }
    | K_CONTAINS { $$ = "CONTAINS"; }
    | K_CONVERT { $$ = "CONVERT"; }
    | K_CORR { $$ = "CORR"; }
    | K_COUNT { $$ = "COUNT"; }
    | K_COVAR_POP { $$ = "COVAR_POP"; }
    | K_COVAR_SAMP { $$ = "COVAR_SAMP"; }
    | K_CUME_DIST { $$ = "CUME_DIST"; }
    | K_CURRENT_COLLATION { $$ = "CURRENT_COLLATION"; }
    | K_CURSOR_NAME { $$ = "CURSOR_NAME"; }
    | K_DATA { $$ = "DATA"; }
    | K_DATETIME_INTERVAL_CODE { $$ = "DATETIME_INTERVAL_CODE"; }
    | K_DATETIME_INTERVAL_PRECISION { $$ = "DATETIME_INTERVAL_PRECISION"; }
    | K_DEFAULTS { $$ = "DEFAULTS"; }
    | K_DEFERRABLE { $$ = "DEFERRABLE"; }
    | K_DEFERRED { $$ = "DEFERRED"; }
    | K_DEFINED { $$ = "DEFINED"; }
    | K_DEFINER { $$ = "DEFINER"; }
    | K_DEGREE { $$ = "DEGREE"; }
    | K_DENSE_RANK { $$ = "DENSE_RANK"; }
    | K_DEPTH { $$ = "DEPTH"; }
    | K_DERIVED { $$ = "DERIVED"; }
    | K_DESC { $$ = "DESC"; }
    | K_DESCRIPTOR { $$ = "DESCRIPTOR"; }
    | K_DIAGNOSTICS { $$ = "DIAGNOSTICS"; }
    | K_DISPATCH { $$ = "DISPATCH"; }
    | K_DOMAIN { $$ = "DOMAIN"; }
    | K_DYNAMIC_FUNCTION { $$ = "DYNAMIC_FUNCTION"; }
    | K_DYNAMIC_FUNCTION_CODE { $$ = "DYNAMIC_FUNCTION_CODE"; }
    | K_EQUALS { $$ = "EQUALS"; }
    | K_EVERY { $$ = "EVERY"; }
    | K_EXCEPTION { $$ = "EXCEPTION"; }
    | K_EXCLUDE { $$ = "EXCLUDE"; }
    | K_EXCLUDING { $$ = "EXCLUDING"; }
    | K_EXP { $$ = "EXP"; }
    | K_EXTRACT { $$ = "EXTRACT"; }
    | K_FINAL { $$ = "FINAL"; }
    | K_FIRST { $$ = "FIRST"; }
    | K_FLOOR { $$ = "FLOOR"; }
    | K_FOLLOWING { $$ = "FOLLOWING"; }
    | K_FORTRAN { $$ = "FORTRAN"; }
    | K_FOUND { $$ = "FOUND"; }
    | K_FUSION { $$ = "FUSION"; }
    | K_G { $$ = "G"; }
    | K_GENERAL { $$ = "GENERAL"; }
    | K_GO { $$ = "GO"; }
    | K_GOTO { $$ = "GOTO"; }
    | K_GRANTED { $$ = "GRANTED"; }
    | K_HIERARCHY { $$ = "HIERARCHY"; }
    | K_IMPLEMENTATION { $$ = "IMPLEMENTATION"; }
    | K_INCLUDING { $$ = "INCLUDING"; }
    | K_INCREMENT { $$ = "INCREMENT"; }
    | K_INITIALLY { $$ = "INITIALLY"; }
    | K_INSTANCE { $$ = "INSTANCE"; }
    | K_INSTANTIABLE { $$ = "INSTANTIABLE"; }
    | K_INTERSECTION { $$ = "INTERSECTION"; }
    | K_INVOKER { $$ = "INVOKER"; }
    | K_K { $$ = "K"; }
    | K_KEY { $$ = "KEY"; }
    | K_KEY_MEMBER { $$ = "KEY_MEMBER"; }
    | K_KEY_TYPE { $$ = "KEY_TYPE"; }
    | K_LAST { $$ = "LAST"; }
    | K_LENGTH { $$ = "LENGTH"; }
    | K_LEVEL { $$ = "LEVEL"; }
    | K_LN { $$ = "LN"; }
    | K_LOCATOR { $$ = "LOCATOR"; }
    | K_LOWER { $$ = "LOWER"; }
    | K_M { $$ = "M"; }
    | K_MAP { $$ = "MAP"; }
    | K_MATCHED { $$ = "MATCHED"; }
    | K_MAX { $$ = "MAX"; }
    | K_MAXVALUE { $$ = "MAXVALUE"; }
    | K_MESSAGE_LENGTH { $$ = "MESSAGE_LENGTH"; }
    | K_MESSAGE_OCTET_LENGTH { $$ = "MESSAGE_OCTET_LENGTH"; }
    | K_MESSAGE_TEXT { $$ = "MESSAGE_TEXT"; }
    | K_MIN { $$ = "MIN"; }
    | K_MINVALUE { $$ = "MINVALUE"; }
    | K_MOD { $$ = "MOD"; }
    | K_MORE { $$ = "MORE"; }
    | K_MUMPS { $$ = "MUMPS"; }
    | K_NAME { $$ = "NAME"; }
    | K_NAMES { $$ = "NAMES"; }
    | K_NESTING { $$ = "NESTING"; }
    | K_NEXT { $$ = "NEXT"; }
    | K_NORMALIZE { $$ = "NORMALIZE"; }
    | K_NORMALIZED { $$ = "NORMALIZED"; }
    | K_NULLABLE { $$ = "NULLABLE"; }
    | K_NULLIF { $$ = "NULLIF"; }
    | K_NULLS { $$ = "NULLS"; }
    | K_NUMBER { $$ = "NUMBER"; }
    | K_OBJECT { $$ = "OBJECT"; }
    | K_OCTETS { $$ = "OCTETS"; }
    | K_OCTET_LENGTH { $$ = "OCTET_LENGTH"; }
    | K_OPTION { $$ = "OPTION"; }
    | K_OPTIONS { $$ = "OPTIONS"; }
    | K_ORDERING { $$ = "ORDERING"; }
    | K_ORDINALITY { $$ = "ORDINALITY"; }
    | K_OTHERS { $$ = "OTHERS"; }
    | K_OVERLAY { $$ = "OVERLAY"; }
    | K_OVERRIDING { $$ = "OVERRIDING"; }
    | K_PAD { $$ = "PAD"; }
    | K_PARAMETER_MODE { $$ = "PARAMETER_MODE"; }
    | K_PARAMETER_NAME { $$ = "PARAMETER_NAME"; }
    | K_PARAMETER_ORDINAL_POSITION { $$ = "PARAMETER_ORDINAL_POSITION"; }
    | K_PARAMETER_SPECIFIC_CATALOG { $$ = "PARAMETER_SPECIFIC_CATALOG"; }
    | K_PARAMETER_SPECIFIC_NAME { $$ = "PARAMETER_SPECIFIC_NAME"; }
    | K_PARAMETER_SPECIFIC_SCHEMA { $$ = "PARAMETER_SPECIFIC_SCHEMA"; }
    | K_PARTIAL { $$ = "PARTIAL"; }
    | K_PASCAL { $$ = "PASCAL"; }
    | K_PATH { $$ = "PATH"; }
    | K_PERCENTILE_CONT { $$ = "PERCENTILE_CONT"; }
    | K_PERCENTILE_DISC { $$ = "PERCENTILE_DISC"; }
    | K_PERCENT_RANK { $$ = "PERCENT_RANK"; }
    | K_PLACING { $$ = "PLACING"; }
    | K_PLI { $$ = "PLI"; }
    | K_POSITION { $$ = "POSITION"; }
    | K_POWER { $$ = "POWER"; }
    | K_PRECEDING { $$ = "PRECEDING"; }
    | K_PRESERVE { $$ = "PRESERVE"; }
    | K_PRIOR { $$ = "PRIOR"; }
    | K_PRIVILEGES { $$ = "PRIVILEGES"; }
    | K_PUBLIC { $$ = "PUBLIC"; }
    | K_RANK { $$ = "RANK"; }
    | K_READ { $$ = "READ"; }
    | K_RELATIVE { $$ = "RELATIVE"; }
    | K_REPEATABLE { $$ = "REPEATABLE"; }
    | K_RESTART { $$ = "RESTART"; }
    | K_RETURNED_CARDINALITY { $$ = "RETURNED_CARDINALITY"; }
    | K_RETURNED_LENGTH { $$ = "RETURNED_LENGTH"; }
    | K_RETURNED_OCTET_LENGTH { $$ = "RETURNED_OCTET_LENGTH"; }
    | K_RETURNED_SQLSTATE { $$ = "RETURNED_SQLSTATE"; }
    | K_ROLE { $$ = "ROLE"; }
    | K_ROUTINE { $$ = "ROUTINE"; }
    | K_ROUTINE_CATALOG { $$ = "ROUTINE_CATALOG"; }
    | K_ROUTINE_NAME { $$ = "ROUTINE_NAME"; }
    | K_ROUTINE_SCHEMA { $$ = "ROUTINE_SCHEMA"; }
    | K_ROW_COUNT { $$ = "ROW_COUNT"; }
    | K_ROW_NUMBER { $$ = "ROW_NUMBER"; }
    | K_SCALE { $$ = "SCALE"; }
    | K_SCHEMA { $$ = "SCHEMA"; }
    | K_SCHEMA_NAME { $$ = "SCHEMA_NAME"; }
    | K_SCOPE_CATALOG { $$ = "SCOPE_CATALOG"; }
    | K_SCOPE_NAME { $$ = "SCOPE_NAME"; }
    | K_SCOPE_SCHEMA { $$ = "SCOPE_SCHEMA"; }
    | K_SECTION { $$ = "SECTION"; }
    | K_SECURITY { $$ = "SECURITY"; }
    | K_SELF { $$ = "SELF"; }
    | K_SEQUENCE { $$ = "SEQUENCE"; }
    | K_SERIALIZABLE { $$ = "SERIALIZABLE"; }
    | K_SERVER_NAME { $$ = "SERVER_NAME"; }
    | K_SESSION { $$ = "SESSION"; }
    | K_SETS { $$ = "SETS"; }
    | K_SIMPLE { $$ = "SIMPLE"; }
    | K_SIZE { $$ = "SIZE"; }
    | K_SOURCE { $$ = "SOURCE"; }
    | K_SPACE { $$ = "SPACE"; }
    | K_SPECIFIC_NAME { $$ = "SPECIFIC_NAME"; }
    | K_SQRT { $$ = "SQRT"; }
    | K_STATE { $$ = "STATE"; }
    | K_STATEMENT { $$ = "STATEMENT"; }
    | K_STDDEV_POP { $$ = "STDDEV_POP"; }
    | K_STDDEV_SAMP { $$ = "STDDEV_SAMP"; }
    | K_STRUCTURE { $$ = "STRUCTURE"; }
    | K_STYLE { $$ = "STYLE"; }
    | K_SUBCLASS_ORIGIN { $$ = "SUBCLASS_ORIGIN"; }
    | K_SUBSTRING { $$ = "SUBSTRING"; }
    | K_SUM { $$ = "SUM"; }
    | K_TABLESAMPLE { $$ = "TABLESAMPLE"; }
    | K_TABLE_NAME { $$ = "TABLE_NAME"; }
    | K_TEMPORARY { $$ = "TEMPORARY"; }
    | K_TIES { $$ = "TIES"; }
    | K_TOP_LEVEL_COUNT { $$ = "TOP_LEVEL_COUNT"; }
    | K_TRANSACTION { $$ = "TRANSACTION"; }
    | K_TRANSACTIONS_COMMITTED { $$ = "TRANSACTIONS_COMMITTED"; }
    | K_TRANSACTIONS_ROLLED_BACK { $$ = "TRANSACTIONS_ROLLED_BACK"; }
    | K_TRANSACTION_ACTIVE { $$ = "TRANSACTION_ACTIVE"; }
    | K_TRANSFORM { $$ = "TRANSFORM"; }
    | K_TRANSFORMS { $$ = "TRANSFORMS"; }
    | K_TRANSLATE { $$ = "TRANSLATE"; }
    | K_TRIGGER_CATALOG { $$ = "TRIGGER_CATALOG"; }
    | K_TRIGGER_NAME { $$ = "TRIGGER_NAME"; }
    | K_TRIGGER_SCHEMA { $$ = "TRIGGER_SCHEMA"; }
    | K_TRIM { $$ = "TRIM"; }
    | K_TYPE { $$ = "TYPE"; }
    | K_UNBOUNDED { $$ = "UNBOUNDED"; }
    | K_UNCOMMITTED { $$ = "UNCOMMITTED"; }
    | K_UNDER { $$ = "UNDER"; }
    | K_UNNAMED { $$ = "UNNAMED"; }
    | K_USAGE { $$ = "USAGE"; }
    | K_USER_DEFINED_TYPE_CATALOG { $$ = "USER_DEFINED_TYPE_CATALOG"; }
    | K_USER_DEFINED_TYPE_CODE { $$ = "USER_DEFINED_TYPE_CODE"; }
    | K_USER_DEFINED_TYPE_NAME { $$ = "USER_DEFINED_TYPE_NAME"; }
    | K_USER_DEFINED_TYPE_SCHEMA { $$ = "USER_DEFINED_TYPE_SCHEMA"; }
    | K_VIEW { $$ = "VIEW"; }
    | K_WORK { $$ = "WORK"; }
    | K_WRITE { $$ = "WRITE"; }
    | K_ZONE { $$ = "ZONE"; }
;

reserved_word:
      K_ADD { $$ = "ADD"; }
    | K_ALL { $$ = "ALL"; }
    | K_ALLOCATE { $$ = "ALLOCATE"; }
    | K_ALTER { $$ = "ALTER"; }
    | K_AND { $$ = "AND"; }
    | K_ANY { $$ = "ANY"; }
    | K_ARE { $$ = "ARE"; }
    | K_ARRAY { $$ = "ARRAY"; }
    | K_AS { $$ = "AS"; }
    | K_ASENSITIVE { $$ = "ASENSITIVE"; }
    | K_ASYMMETRIC { $$ = "ASYMMETRIC"; }
    | K_AT { $$ = "AT"; }
    | K_ATOMIC { $$ = "ATOMIC"; }
    | K_AUTHORIZATION { $$ = "AUTHORIZATION"; }
    | K_BEGIN { $$ = "BEGIN"; }
    | K_BETWEEN { $$ = "BETWEEN"; }
    | K_BIGINT { $$ = "BIGINT"; }
    | K_BINARY { $$ = "BINARY"; }
    | K_BLOB { $$ = "BLOB"; }
    | K_BOOLEAN { $$ = "BOOLEAN"; }
    | K_BOTH { $$ = "BOTH"; }
    | K_BY { $$ = "BY"; }
    | K_CALL { $$ = "CALL"; }
    | K_CALLED { $$ = "CALLED"; }
    | K_CASCADED { $$ = "CASCADED"; }
    | K_CASE { $$ = "CASE"; }
    | K_CAST { $$ = "CAST"; }
    | K_CHAR { $$ = "CHAR"; }
    | K_CHARACTER { $$ = "CHARACTER"; }
    | K_CHECK { $$ = "CHECK"; }
    | K_CLOB { $$ = "CLOB"; }
    | K_CLOSE { $$ = "CLOSE"; }
    | K_COLLATE { $$ = "COLLATE"; }
    | K_COLUMN { $$ = "COLUMN"; }
    | K_COMMIT { $$ = "COMMIT"; }
    | K_CONNECT { $$ = "CONNECT"; }
    | K_CONSTRAINT { $$ = "CONSTRAINT"; }
    | K_CONTINUE { $$ = "CONTINUE"; }
    | K_CORRESPONDING { $$ = "CORRESPONDING"; }
    | K_CREATE { $$ = "CREATE"; }
    | K_CROSS { $$ = "CROSS"; }
    | K_CUBE { $$ = "CUBE"; }
    | K_CURRENT { $$ = "CURRENT"; }
    | K_CURRENT_DATE { $$ = "CURRENT_DATE"; }
    | K_CURRENT_DEFAULT_TRANSFORM_GROUP { $$ = "CURRENT_DEFAULT_TRANSFORM_GROUP"; }
    | K_CURRENT_PATH { $$ = "CURRENT_PATH"; }
    | K_CURRENT_ROLE { $$ = "CURRENT_ROLE"; }
    | K_CURRENT_TIME { $$ = "CURRENT_TIME"; }
    | K_CURRENT_TIMESTAMP { $$ = "CURRENT_TIMESTAMP"; }
    | K_CURRENT_TRANSFORM_GROUP_FOR_TYPE { $$ = "CURRENT_TRANSFORM_GROUP_FOR_TYPE"; }
    | K_CURRENT_USER { $$ = "CURRENT_USER"; }
    | K_CURSOR { $$ = "CURSOR"; }
    | K_CYCLE { $$ = "CYCLE"; }
    | K_DATE { $$ = "DATE"; }
    | K_DAY { $$ = "DAY"; }
    | K_DEALLOCATE { $$ = "DEALLOCATE"; }
    | K_DEC { $$ = "DEC"; }
    | K_DECIMAL { $$ = "DECIMAL"; }
    | K_DECLARE { $$ = "DECLARE"; }
    | K_DEFAULT { $$ = "DEFAULT"; }
    | K_DELETE { $$ = "DELETE"; }
    | K_DEREF { $$ = "DEREF"; }
    | K_DESCRIBE { $$ = "DESCRIBE"; }
    | K_DETERMINISTIC { $$ = "DETERMINISTIC"; }
    | K_DISCONNECT { $$ = "DISCONNECT"; }
    | K_DISTINCT { $$ = "DISTINCT"; }
    | K_DOUBLE { $$ = "DOUBLE"; }
    | K_DROP { $$ = "DROP"; }
    | K_DYNAMIC { $$ = "DYNAMIC"; }
    | K_EACH { $$ = "EACH"; }
    | K_ELEMENT { $$ = "ELEMENT"; }
    | K_ELSE { $$ = "ELSE"; }
    | K_END { $$ = "END"; }
    | K_END_EXEC { $$ = "END-EXEC"; }
    | K_ESCAPE { $$ = "ESCAPE"; }
    | K_EXCEPT { $$ = "EXCEPT"; }
    | K_EXEC { $$ = "EXEC"; }
    | K_EXECUTE { $$ = "EXECUTE"; }
    | K_EXISTS { $$ = "EXISTS"; }
    | K_EXTERNAL { $$ = "EXTERNAL"; }
    | K_FALSE { $$ = "FALSE"; }
    | K_FETCH { $$ = "FETCH"; }
    | K_FILTER { $$ = "FILTER"; }
    | K_FLOAT { $$ = "FLOAT"; }
    | K_FOR { $$ = "FOR"; }
    | K_FOREIGN { $$ = "FOREIGN"; }
    | K_FREE { $$ = "FREE"; }
    | K_FROM { $$ = "FROM"; }
    | K_FULL { $$ = "FULL"; }
    | K_FUNCTION { $$ = "FUNCTION"; }
    | K_GET { $$ = "GET"; }
    | K_GLOBAL { $$ = "GLOBAL"; }
    | K_GRANT { $$ = "GRANT"; }
    | K_GROUP { $$ = "GROUP"; }
    | K_GROUPING { $$ = "GROUPING"; }
    | K_HAVING { $$ = "HAVING"; }
    | K_HOLD { $$ = "HOLD"; }
    | K_HOUR { $$ = "HOUR"; }
    | K_IDENTITY { $$ = "IDENTITY"; }
    | K_IMMEDIATE { $$ = "IMMEDIATE"; }
    | K_IN { $$ = "IN"; }
    | K_INDICATOR { $$ = "INDICATOR"; }
    | K_INNER { $$ = "INNER"; }
    | K_INOUT { $$ = "INOUT"; }
    | K_INPUT { $$ = "INPUT"; }
    | K_INSENSITIVE { $$ = "INSENSITIVE"; }
    | K_INSERT { $$ = "INSERT"; }
    | K_INT { $$ = "INT"; }
    | K_INTEGER { $$ = "INTEGER"; }
    | K_INTERSECT { $$ = "INTERSECT"; }
    | K_INTERVAL { $$ = "INTERVAL"; }
    | K_INTO { $$ = "INTO"; }
    | K_IS { $$ = "IS"; }
    | K_ISOLATION { $$ = "ISOLATION"; }
    | K_JOIN { $$ = "JOIN"; }
    | K_LANGUAGE { $$ = "LANGUAGE"; }
    | K_LARGE { $$ = "LARGE"; }
    | K_LATERAL { $$ = "LATERAL"; }
    | K_LEADING { $$ = "LEADING"; }
    | K_LEFT { $$ = "LEFT"; }
    | K_LIKE { $$ = "LIKE"; }
    | K_LOCAL { $$ = "LOCAL"; }
    | K_LOCALTIME { $$ = "LOCALTIME"; }
    | K_LOCALTIMESTAMP { $$ = "LOCALTIMESTAMP"; }
    | K_MATCH { $$ = "MATCH"; }
    | K_MEMBER { $$ = "MEMBER"; }
    | K_MERGE { $$ = "MERGE"; }
    | K_METHOD { $$ = "METHOD"; }
    | K_MINUTE { $$ = "MINUTE"; }
    | K_MODIFIES { $$ = "MODIFIES"; }
    | K_MODULE { $$ = "MODULE"; }
    | K_MONTH { $$ = "MONTH"; }
    | K_MULTISET { $$ = "MULTISET"; }
    | K_NATIONAL { $$ = "NATIONAL"; }
    | K_NATURAL { $$ = "NATURAL"; }
    | K_NCHAR { $$ = "NCHAR"; }
    | K_NCLOB { $$ = "NCLOB"; }
    | K_NEW { $$ = "NEW"; }
    | K_NO { $$ = "NO"; }
    | K_NONE { $$ = "NONE"; }
    | K_NOT { $$ = "NOT"; }
    | K_NULL { $$ = "NULL"; }
    | K_NUMERIC { $$ = "NUMERIC"; }
    | K_OF { $$ = "OF"; }
    | K_OLD { $$ = "OLD"; }
    | K_ON { $$ = "ON"; }
    | K_ONLY { $$ = "ONLY"; }
    | K_OPEN { $$ = "OPEN"; }
    | K_OR { $$ = "OR"; }
    | K_ORDER { $$ = "ORDER"; }
    | K_OUT { $$ = "OUT"; }
    | K_OUTER { $$ = "OUTER"; }
    | K_OUTPUT { $$ = "OUTPUT"; }
    | K_OVER { $$ = "OVER"; }
    | K_OVERLAPS { $$ = "OVERLAPS"; }
    | K_PARAMETER { $$ = "PARAMETER"; }
    | K_PARTITION { $$ = "PARTITION"; }
    | K_PRECISION { $$ = "PRECISION"; }
    | K_PREPARE { $$ = "PREPARE"; }
    | K_PRIMARY { $$ = "PRIMARY"; }
    | K_PROCEDURE { $$ = "PROCEDURE"; }
    | K_RANGE { $$ = "RANGE"; }
    | K_READS { $$ = "READS"; }
    | K_REAL { $$ = "REAL"; }
    | K_RECURSIVE { $$ = "RECURSIVE"; }
    | K_REF { $$ = "REF"; }
    | K_REFERENCES { $$ = "REFERENCES"; }
    | K_REFERENCING { $$ = "REFERENCING"; }
    | K_REGR_AVGX { $$ = "REGR_AVGX"; }
    | K_REGR_AVGY { $$ = "REGR_AVGY"; }
    | K_REGR_COUNT { $$ = "REGR_COUNT"; }
    | K_REGR_INTERCEPT { $$ = "REGR_INTERCEPT"; }
    | K_REGR_R2 { $$ = "REGR_R2"; }
    | K_REGR_SLOPE { $$ = "REGR_SLOPE"; }
    | K_REGR_SXX { $$ = "REGR_SXX"; }
    | K_REGR_SXY { $$ = "REGR_SXY"; }
    | K_REGR_SYY { $$ = "REGR_SYY"; }
    | K_RELEASE { $$ = "RELEASE"; }
    | K_RESULT { $$ = "RESULT"; }
    | K_RETURN { $$ = "RETURN"; }
    | K_RETURNS { $$ = "RETURNS"; }
    | K_REVOKE { $$ = "REVOKE"; }
    | K_RIGHT { $$ = "RIGHT"; }
    | K_ROLLBACK { $$ = "ROLLBACK"; }
    | K_ROLLUP { $$ = "ROLLUP"; }
    | K_ROW { $$ = "ROW"; }
    | K_ROWS { $$ = "ROWS"; }
    | K_SAVEPOINT { $$ = "SAVEPOINT"; }
    | K_SCROLL { $$ = "SCROLL"; }
    | K_SEARCH { $$ = "SEARCH"; }
    | K_SECOND { $$ = "SECOND"; }
    | K_SELECT { $$ = "SELECT"; }
    | K_SENSITIVE { $$ = "SENSITIVE"; }
    | K_SESSION_USER { $$ = "SESSION_USER"; }
    | K_SET { $$ = "SET"; }
    | K_SIMILAR { $$ = "SIMILAR"; }
    | K_SMALLINT { $$ = "SMALLINT"; }
    | K_SOME { $$ = "SOME"; }
    | K_SPECIFIC { $$ = "SPECIFIC"; }
    | K_SPECIFICTYPE { $$ = "SPECIFICTYPE"; }
    | K_SQL { $$ = "SQL"; }
    | K_SQLEXCEPTION { $$ = "SQLEXCEPTION"; }
    | K_SQLSTATE { $$ = "SQLSTATE"; }
    | K_SQLWARNING { $$ = "SQLWARNING"; }
    | K_START { $$ = "START"; }
    | K_STATIC { $$ = "STATIC"; }
    | K_SUBMULTISET { $$ = "SUBMULTISET"; }
    | K_SYMMETRIC { $$ = "SYMMETRIC"; }
    | K_SYSTEM { $$ = "SYSTEM"; }
    | K_SYSTEM_USER { $$ = "SYSTEM_USER"; }
    | K_TABLE { $$ = "TABLE"; }
    | K_THEN { $$ = "THEN"; }
    | K_TIME { $$ = "TIME"; }
    | K_TIMESTAMP { $$ = "TIMESTAMP"; }
    | K_TIMEZONE_HOUR { $$ = "TIMEZONE_HOUR"; }
    | K_TIMEZONE_MINUTE { $$ = "TIMEZONE_MINUTE"; }
    | K_TO { $$ = "TO"; }
    | K_TRAILING { $$ = "TRAILING"; }
    | K_TRANSLATION { $$ = "TRANSLATION"; }
    | K_TREAT { $$ = "TREAT"; }
    | K_TRIGGER { $$ = "TRIGGER"; }
    | K_TRUE { $$ = "TRUE"; }
    | K_UESCAPE { $$ = "UESCAPE"; }
    | K_UNION { $$ = "UNION"; }
    | K_UNIQUE { $$ = "UNIQUE"; }
    | K_UNKNOWN { $$ = "UNKNOWN"; }
    | K_UNNEST { $$ = "UNNEST"; }
    | K_UPDATE { $$ = "UPDATE"; }
    | K_UPPER { $$ = "UPPER"; }
    | K_USER { $$ = "USER"; }
    | K_USING { $$ = "USING"; }
    | K_VALUE { $$ = "VALUE"; }
    | K_VALUES { $$ = "VALUES"; }
    | K_VAR_POP { $$ = "VAR_POP"; }
    | K_VAR_SAMP { $$ = "VAR_SAMP"; }
    | K_VARCHAR { $$ = "VARCHAR"; }
    | K_VARYING { $$ = "VARYING"; }
    | K_WHEN { $$ = "WHEN"; }
    | K_WHENEVER { $$ = "WHENEVER"; }
    | K_WHERE { $$ = "WHERE"; }
    | K_WIDTH_BUCKET { $$ = "WIDTH_BUCKET"; }
    | K_WINDOW { $$ = "WINDOW"; }
    | K_WITH { $$ = "WITH"; }
    | K_WITHIN { $$ = "WITHIN"; }
    | K_WITHOUT { $$ = "WITHOUT"; }
    | K_YEAR { $$ = "YEAR"; }
;

select_statement:
    K_SELECT error {
        ABORT("%s", "Missing expression list after SELECT");
    }
    |
    K_SELECT column_expression_list K_FROM identifier error {
        EagleLinkedList_DeleteWithItems($2);
        EagleDbSqlValue_Delete($4);
        
        ABORT("%s", "Unexpected token after FROM clause");
    }
    |
    K_SELECT column_expression_list K_FROM identifier where_expression {
        EagleDbSqlSelect *select = EagleDbSqlSelect_New();
        select->selectExpressions = $2;
        char *name = strdup(((EagleDbSqlValue*) $4)->value.identifier);
        EagleDbSqlValue_Delete($4);
        select->tableName = name;
        select->whereExpression = $5;
        $$ = select;
    }
;

column_expression_list:
    column_expression {
        $$ = EagleLinkedList_New();
        EagleLinkedListItem *item = EagleLinkedListItem_New($1, EagleTrue, (void(*)(void*)) EagleDbSqlExpression_DeleteRecursive);
        EagleLinkedList_add($$, item);
    }
    |
    column_expression_list T_COMMA column_expression {
        EagleLinkedListItem *item = EagleLinkedListItem_New($3, EagleTrue, (void(*)(void*)) EagleDbSqlExpression_DeleteRecursive);
        EagleLinkedList_add($1, item);
    }
;

column_expression:
      T_ASTERISK { $$ = EagleDbSqlValue_NewWithAsterisk(); }
    | expression
;

where_expression:
      /* empty */ { $$ = NULL; }
    | K_WHERE expression { $$ = $2; }
;

expression:
      value

    /* grouping */
    | T_BRACKET_OPEN expression T_BRACKET_CLOSE { $$ = EagleDbSqlUnaryExpression_New(EagleDbSqlUnaryExpressionOperatorGrouping, $2); }

    /* unary operators */
    | T_MINUS expression { $$ = EagleDbSqlUnaryExpression_New(EagleDbSqlUnaryExpressionOperatorNegate, $2); }
    | K_NOT expression { $$ = EagleDbSqlUnaryExpression_New(EagleDbSqlUnaryExpressionOperatorNot, $2); }

    /* logical operators */
    | expression K_OR expression { $$ = EagleDbSqlBinaryExpression_New($1, EagleDbSqlBinaryExpressionOperatorOr, $3); }
    | expression K_AND expression { $$ = EagleDbSqlBinaryExpression_New($1, EagleDbSqlBinaryExpressionOperatorAnd, $3); }

    /* arithmetic operators */
    | expression T_PLUS expression { $$ = EagleDbSqlBinaryExpression_New($1, EagleDbSqlBinaryExpressionOperatorPlus, $3); }
    | expression T_ASTERISK expression { $$ = EagleDbSqlBinaryExpression_New($1, EagleDbSqlBinaryExpressionOperatorMultiply, $3); }
    | expression T_MINUS expression { $$ = EagleDbSqlBinaryExpression_New($1, EagleDbSqlBinaryExpressionOperatorMinus, $3); }
    | expression T_DIVIDE expression { $$ = EagleDbSqlBinaryExpression_New($1, EagleDbSqlBinaryExpressionOperatorDivide, $3); }
    | expression T_MODULUS expression { $$ = EagleDbSqlBinaryExpression_New($1, EagleDbSqlBinaryExpressionOperatorModulus, $3); }

    /* comparison operators */
    | expression T_EQUALS expression { $$ = EagleDbSqlBinaryExpression_New($1, EagleDbSqlBinaryExpressionOperatorEquals, $3); }
    | expression T_NOT_EQUALS expression { $$ = EagleDbSqlBinaryExpression_New($1, EagleDbSqlBinaryExpressionOperatorNotEquals, $3); }
    | expression T_GREATER_THAN expression { $$ = EagleDbSqlBinaryExpression_New($1, EagleDbSqlBinaryExpressionOperatorGreaterThan, $3); }
    | expression T_LESS_THAN expression { $$ = EagleDbSqlBinaryExpression_New($1, EagleDbSqlBinaryExpressionOperatorLessThan, $3); }
    | expression T_GREATER_THAN_EQUAL expression { $$ = EagleDbSqlBinaryExpression_New($1, EagleDbSqlBinaryExpressionOperatorGreaterThanEqual, $3); }
    | expression T_LESS_THAN_EQUAL expression { $$ = EagleDbSqlBinaryExpression_New($1, EagleDbSqlBinaryExpressionOperatorLessThanEqual, $3); }
;

value:
    integer | identifier | string_literal
;

integer:
    INTEGER {
        char *lastToken = EagleDbParser_lastToken(parser);
        int value = atoi(lastToken);
        $$ = EagleDbSqlValue_NewWithInteger(value);
    }
;

identifier:
    nonreserved_word {
        char *lower = EagleUtils_ToLowerCaseCopy($1);
        $$ = EagleDbSqlValue_NewWithIdentifier(lower);
        free(lower);
    }
    | IDENTIFIER {
        char *lastToken = EagleDbParser_lastToken(parser);
        $$ = EagleDbSqlValue_NewWithIdentifier(lastToken);
    }
;

string_literal:
    STRING_LITERAL {
        char *lastToken = EagleDbParser_lastToken(parser);
        $$ = EagleDbSqlValue_NewWithString(lastToken, EagleTrue);
    }
;

%%
