%{

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

/* variable tokens */
%token IDENTIFIER
%token INTEGER

/* fixed tokens */
%token T_ASTERISK
%token T_PLUS
%token T_EQUALS
%token T_END
%token T_COMMA
%token T_BRACKET_OPEN
%token T_BRACKET_CLOSE

%%

/*
 ( statement? T_END )?
 */
input:
    {
        EagleDbParser *p = EagleDbParser_Get();
        p->yyparse_ast = NULL;
        p->yystatementtype = EagleDbSqlStatementTypeNone;
    }
    |
    T_END {
        EagleDbParser *p = EagleDbParser_Get();
        p->yyparse_ast = NULL;
        p->yystatementtype = EagleDbSqlStatementTypeNone;
    }
    |
    statement {
        EagleDbParser *p = EagleDbParser_Get();
        p->yyparse_ast = yyreturn_pop();
    }
    T_END
;

/*
 select_statement | create_table_statement
 */
statement:
    select_statement {
        /* bubble up yyreturn */
        EagleDbParser *p = EagleDbParser_Get();
        p->yystatementtype = EagleDbSqlStatementTypeSelect;
    }
    |
    create_table_statement {
        /* bubble up yyreturn */
        EagleDbParser *p = EagleDbParser_Get();
        p->yystatementtype = EagleDbSqlStatementTypeCreateTable;
    }
;

/*
 K_CREATE K_TABLE IDENTIFIER T_BRACKET_OPEN column_definition_list T_BRACKET_CLOSE
 */
create_table_statement:
    K_CREATE K_TABLE {
        yyreturn_push(yyobj_push((void*) EagleDbTable_New(NULL)));
    }
    IDENTIFIER {
        ((EagleDbTable*) yyreturn_current())->name = yyobj_push(strdup(yytext_last));
    }
    T_BRACKET_OPEN
    column_definition_list {
        EagleDbParser *p = EagleDbParser_Get();
        void *last = yyreturn_pop();
        EagleDbTable_setColumns((EagleDbTable*) yyreturn_current(), last, p->yylist_length);
    }
    T_BRACKET_CLOSE
;

/*
 column_definition next_column_definition
 */
column_definition_list:
    {
        yyobj_push(yylist_new());
    }
    column_definition {
        void *last = yyreturn_pop();
        yylist_push(last);
    }
    next_column_definition
    {
        EagleDbParser *p = EagleDbParser_Get();
        yyreturn_push(p->yylist);
    }
;

/*
 IDENTIFIER data_type
 */
column_definition:
    IDENTIFIER {
        yyreturn_push(yyobj_push((void*) EagleDbColumn_New(NULL, EagleDataTypeInteger)));
        ((EagleDbColumn*) yyreturn_current())->name = yyobj_push(strdup(yytext_last));
    }
    data_type {
        EagleDbParser *p = EagleDbParser_Get();
        ((EagleDbColumn*) yyreturn_current())->type = p->yy_data_type;
    }
;

/*
 K_INTEGER | K_TEXT
 */
data_type:
    K_INTEGER {
        EagleDbParser *p = EagleDbParser_Get();
        p->yy_data_type = EagleDataTypeInteger;
    }
    |
    K_TEXT {
        EagleDbParser *p = EagleDbParser_Get();
        p->yy_data_type = EagleDataTypeText;
    }
;

/*
 ( T_COMMA column_definition next_column_definition )?
 */
next_column_definition:
    |
    T_COMMA column_definition {
        void *last = yyreturn_pop();
        yylist_push(last);
    }
    next_column_definition
;

/*
 K_SELECT column_expression_list K_FROM IDENTIFIER where_expression
 */
select_statement:
    K_SELECT {
        yyreturn_push(yyobj_push((void*) EagleDbSqlSelect_New()));
    }
    column_expression_list {
        EagleDbParser *p = EagleDbParser_Get();
        void *last = yyreturn_pop();
        ((EagleDbSqlSelect*) yyreturn_current())->selectExpressions = last;
        ((EagleDbSqlSelect*) yyreturn_current())->allocatedSelectExpressions = p->yylist_length;
        ((EagleDbSqlSelect*) yyreturn_current())->usedSelectExpressions = p->yylist_length;
    }
    K_FROM IDENTIFIER {
        ((EagleDbSqlSelect*) yyreturn_current())->tableName = yyobj_push(strdup(yytext_last));
    }
    where_expression {
        void *last = yyreturn_pop();
        ((EagleDbSqlSelect*) yyreturn_current())->whereExpression = last;
    }
;

/*
 column_expression next_column_expression
 */
column_expression_list:
    {
        yyobj_push(yylist_new());
    }
    column_expression {
        void *last = yyreturn_pop();
        yylist_push(last);
    }
    next_column_expression
    {
        EagleDbParser *p = EagleDbParser_Get();
        yyreturn_push(p->yylist);
    }
;

/*
 ( T_COMMA column_expression next_column_expression )?
 */
next_column_expression:
    |
    T_COMMA column_expression {
        void *last = yyreturn_pop();
        yylist_push(last);
    }
    next_column_expression
;

/*
 ( K_WHERE expression )?
 */
where_expression:
    {
        /* no where clause */
        yyreturn_push(NULL);
    }
    |
    K_WHERE expression {
        /* bubble up expression */
    }
;

/*
 T_ASTERISK | expression
 */
column_expression:
    T_ASTERISK {
        yyreturn_push(yyobj_push((void*) EagleDbSqlValue_NewWithAsterisk()));
    }
    |
    expression {
        /* bubble up yyreturn */
    }
;

/*
 value ( ( T_PLUS | T_EQUALS ) value )?
 */
expression:
    value {
        /* bubble up yyreturn */
    }
    |
    value {
        void *last = yyreturn_pop();
        yyreturn_push(yyobj_push((void*) EagleDbSqlBinaryExpression_New((EagleDbSqlExpression*) last, 0, NULL)));
    }
    T_PLUS {
        ((EagleDbSqlBinaryExpression*) yyreturn_current())->op = EagleDbSqlExpressionOperatorPlus;
    }
    value {
        void *last = yyreturn_pop();
        ((EagleDbSqlBinaryExpression*) yyreturn_current())->right = (EagleDbSqlExpression*) last;
    }
    |
    value {
        void *last = yyreturn_pop();
        yyreturn_push(yyobj_push((void*) EagleDbSqlBinaryExpression_New((EagleDbSqlExpression*) last, 0, NULL)));
    }
    T_EQUALS {
        ((EagleDbSqlBinaryExpression*) yyreturn_current())->op = EagleDbSqlExpressionOperatorEquals;
    }
    value {
        void *last = yyreturn_pop();
        ((EagleDbSqlBinaryExpression*) yyreturn_current())->right = (EagleDbSqlExpression*) last;
    }
;

/*
 INTEGER | IDENTIFIER
 */
value:
    INTEGER {
        yyreturn_push(yyobj_push((void*) EagleDbSqlValue_NewWithInteger(atoi(yytext_last))));
    }
    |
    IDENTIFIER {
        yyreturn_push(yyobj_push((void*) EagleDbSqlValue_NewWithIdentifier(yytext_last)));
    }
;

%%
