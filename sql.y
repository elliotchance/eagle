%{

    #define YYSTYPE void*
    
    #include <math.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #include "EagleDbSqlSelect.h"
    #include "EagleDbSqlBinaryExpression.h"
    #include "EagleDbTable.h"
    #include "EagleDbColumn.h"
    #include "EagleDbInstance.h"
    #include "EagleMemory.h"
    #include "EagleDbParser.h"
    #include "EagleDataType.h"
    #include "EagleDbSqlInsert.h"
    
    int yylex(void);
    
    #define ABORT(fmt, ...) { \
    char msg[1024]; \
    sprintf(msg, fmt, __VA_ARGS__); \
    yyerror(msg); \
    YYERROR; \
    }

%}

/* be more versbose about error messages */
%error-verbose

/* keywords */
%token K_CREATE
%token K_FROM
%token K_INTEGER
%token K_SELECT
%token K_TABLE
%token K_TEXT
%token K_WHERE
%token K_VALUES
%token K_INSERT
%token K_INTO

/* variable tokens */
%token IDENTIFIER
%token INTEGER

/* fixed tokens */
%token T_END           ";"
%token T_COMMA         ","
%token T_BRACKET_OPEN  "("
%token T_BRACKET_CLOSE ")"

/* operators */
%left  T_PLUS          "+"
%left  T_ASTERISK      "*"
%right T_EQUALS        "="

%token END 0 "end of file"

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
    END {
        EagleDbParser *p = EagleDbParser_Get();
        p->yyparse_ast = NULL;
        p->yystatementtype = EagleDbSqlStatementTypeNone;
    }
    |
    T_END END {
        EagleDbParser *p = EagleDbParser_Get();
        p->yyparse_ast = NULL;
        p->yystatementtype = EagleDbSqlStatementTypeNone;
    }
    |
    statement END {
        EagleDbParser *p = EagleDbParser_Get();
        p->yyparse_ast = $1;
    }
    |
    statement T_END END {
        EagleDbParser *p = EagleDbParser_Get();
        p->yyparse_ast = $1;
    }
;

statement:
      select_statement { EagleDbParser_SetStatementType(EagleDbSqlStatementTypeSelect); } 
    | create_table_statement { EagleDbParser_SetStatementType(EagleDbSqlStatementTypeCreateTable); }
    | insert_statement { EagleDbParser_SetStatementType(EagleDbSqlStatementTypeInsert); }
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
    K_CREATE | K_FROM | K_INTEGER | K_SELECT | K_TABLE | K_TEXT | K_WHERE | K_VALUES | K_INSERT | K_INTO
;

select_statement:
    K_SELECT error {
        EagleDbParser_SetStatementType(EagleDbSqlStatementTypeSelect);
        ABORT("%s", "Missing expression list after SELECT");
    }
    |
    K_SELECT column_expression_list K_FROM identifier error {
        EagleLinkedList_DeleteWithItems($2);
        EagleDbSqlValue_Delete($4);
        
        EagleDbParser_SetStatementType(EagleDbSqlStatementTypeSelect);
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
    | expression T_PLUS expression { $$ = EagleDbSqlBinaryExpression_New($1, EagleDbSqlExpressionOperatorPlus, $3); }
    | expression T_ASTERISK expression { $$ = EagleDbSqlBinaryExpression_New($1, EagleDbSqlExpressionOperatorMultiply, $3); }
    | expression T_EQUALS expression { $$ = EagleDbSqlBinaryExpression_New($1, EagleDbSqlExpressionOperatorEquals, $3); }
;

value:
    integer | identifier
;

integer:
    INTEGER { $$ = EagleDbSqlValue_NewWithInteger(atoi(EagleDbParser_LastToken())); }
;

identifier:
    IDENTIFIER { $$ = EagleDbSqlValue_NewWithIdentifier(EagleDbParser_LastToken()); }
;

%%
