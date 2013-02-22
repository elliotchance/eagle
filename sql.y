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

/*%union {
    int value;
    SExpression *expression;
}*/

/* keywords */
%token K_CREATE   "CREATE"
%token K_FROM       "FROM"
%token K_INTEGER "INTEGER"
%token K_SELECT   "SELECT"
%token K_TABLE     "TABLE"
%token K_TEXT       "TEXT"
%token K_WHERE     "WHERE"
%token K_VALUES   "VALUES"
%token K_INSERT   "INSERT"
%token K_INTO       "INTO"

/* variable tokens */
%token IDENTIFIER "identifier"
%token INTEGER       "integer"

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
%right T_ASTERISK        "*"  T_DIVIDE    "/"  T_MODULUS             "%"

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
    identifier data_type {
        $$ = EagleDbColumn_New(((EagleDbSqlValue*) $1)->value.identifier, *((int*) $2));
        EagleDbSqlValue_Delete($1);
        EagleMemory_Free($2);
    }
;

data_type:
      K_INTEGER { $$ = EagleData_Int(EagleDataTypeInteger); }
    | K_TEXT { $$ = EagleData_Int(EagleDataTypeText); }
;

keyword:
      K_CREATE { $$ = "CREATE"; }
    | K_FROM { $$ = "FROM"; }
    | K_INTEGER { $$ = "INTEGER"; }
    | K_SELECT { $$ = "SELECT"; }
    | K_TABLE { $$ = "TABLE"; }
    | K_TEXT { $$ = "TEXT"; }
    | K_WHERE { $$ = "WHERE"; }
    | K_VALUES { $$ = "VALUES"; }
    | K_INSERT { $$ = "INSERT"; }
    | K_INTO { $$ = "INTO"; }
    | K_OR { $$ = "OR"; }
    | K_AND { $$ = "AND"; }
    | K_NOT { $$ = "NOT"; }
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
    integer | identifier
;

integer:
    INTEGER {
        char *lastToken = EagleDbParser_lastToken(parser);
        int value = atoi(lastToken);
        $$ = EagleDbSqlValue_NewWithInteger(value);
    }
;

identifier:
    IDENTIFIER {
        char *lastToken = EagleDbParser_lastToken(parser);
        $$ = EagleDbSqlValue_NewWithIdentifier(lastToken);
    }
;

%%
