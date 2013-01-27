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
    
    int yylex(void);

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
        p->yyparse_ast = EagleDbParser_PopReturn();
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
        EagleDbParser_AddReturn(EagleDbParser_AddObject((void*) EagleDbTable_New(NULL)));
    }
    IDENTIFIER {
        ((EagleDbTable*) EagleDbParser_CurrentReturn())->name = EagleDbParser_AddObject(strdup(EagleDbParser_LastToken()));
    }
    T_BRACKET_OPEN
    column_definition_list {
        EagleDbParser *p = EagleDbParser_Get();
        void *last = EagleDbParser_PopReturn();
        EagleDbTable_setColumns((EagleDbTable*) EagleDbParser_CurrentReturn(), last, p->yylist_length);
    }
    T_BRACKET_CLOSE
;

/*
 column_definition next_column_definition
 */
column_definition_list:
    {
        EagleDbParser_AddObject(yylist_new());
    }
    column_definition {
        void *last = EagleDbParser_PopReturn();
        yylist_push(last);
    }
    next_column_definition
    {
        EagleDbParser *p = EagleDbParser_Get();
        EagleDbParser_AddReturn(p->yylist);
    }
;

/*
 IDENTIFIER data_type
 */
column_definition:
    IDENTIFIER {
        EagleDbParser_AddReturn(EagleDbParser_AddObject((void*) EagleDbColumn_New(NULL, EagleDataTypeInteger)));
        ((EagleDbColumn*) EagleDbParser_CurrentReturn())->name = EagleDbParser_AddObject(strdup(EagleDbParser_LastToken()));
    }
    data_type {
        EagleDbParser *p = EagleDbParser_Get();
        ((EagleDbColumn*) EagleDbParser_CurrentReturn())->type = p->yy_data_type;
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
        void *last = EagleDbParser_PopReturn();
        yylist_push(last);
    }
    next_column_definition
;

/*
 K_SELECT column_expression_list K_FROM IDENTIFIER where_expression
 */
select_statement:
    K_SELECT {
        EagleDbParser_AddReturn(EagleDbParser_AddObject((void*) EagleDbSqlSelect_New()));
    }
    column_expression_list {
        EagleDbParser *p = EagleDbParser_Get();
        void *last = EagleDbParser_PopReturn();
        ((EagleDbSqlSelect*) EagleDbParser_CurrentReturn())->selectExpressions = last;
        ((EagleDbSqlSelect*) EagleDbParser_CurrentReturn())->allocatedSelectExpressions = p->yylist_length;
        ((EagleDbSqlSelect*) EagleDbParser_CurrentReturn())->usedSelectExpressions = p->yylist_length;
    }
    K_FROM IDENTIFIER {
        ((EagleDbSqlSelect*) EagleDbParser_CurrentReturn())->tableName = EagleDbParser_AddObject(strdup(EagleDbParser_LastToken()));
    }
    where_expression {
        void *last = EagleDbParser_PopReturn();
        ((EagleDbSqlSelect*) EagleDbParser_CurrentReturn())->whereExpression = last;
    }
;

/*
 column_expression next_column_expression
 */
column_expression_list:
    {
        EagleDbParser_AddObject(yylist_new());
    }
    column_expression {
        void *last = EagleDbParser_PopReturn();
        yylist_push(last);
    }
    next_column_expression
    {
        EagleDbParser *p = EagleDbParser_Get();
        EagleDbParser_AddReturn(p->yylist);
    }
;

/*
 ( T_COMMA column_expression next_column_expression )?
 */
next_column_expression:
    |
    T_COMMA column_expression {
        void *last = EagleDbParser_PopReturn();
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
        EagleDbParser_AddReturn(NULL);
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
        EagleDbParser_AddReturn(EagleDbParser_AddObject((void*) EagleDbSqlValue_NewWithAsterisk()));
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
        void *last = EagleDbParser_PopReturn();
        EagleDbParser_AddReturn(EagleDbParser_AddObject((void*) EagleDbSqlBinaryExpression_New((EagleDbSqlExpression*) last, 0, NULL)));
    }
    T_PLUS {
        ((EagleDbSqlBinaryExpression*) EagleDbParser_CurrentReturn())->op = EagleDbSqlExpressionOperatorPlus;
    }
    value {
        void *last = EagleDbParser_PopReturn();
        ((EagleDbSqlBinaryExpression*) EagleDbParser_CurrentReturn())->right = (EagleDbSqlExpression*) last;
    }
    |
    value {
        void *last = EagleDbParser_PopReturn();
        EagleDbParser_AddReturn(EagleDbParser_AddObject((void*) EagleDbSqlBinaryExpression_New((EagleDbSqlExpression*) last, 0, NULL)));
    }
    T_EQUALS {
        ((EagleDbSqlBinaryExpression*) EagleDbParser_CurrentReturn())->op = EagleDbSqlExpressionOperatorEquals;
    }
    value {
        void *last = EagleDbParser_PopReturn();
        ((EagleDbSqlBinaryExpression*) EagleDbParser_CurrentReturn())->right = (EagleDbSqlExpression*) last;
    }
;

/*
 INTEGER | IDENTIFIER
 */
value:
    INTEGER {
        EagleDbParser_AddReturn(EagleDbParser_AddObject((void*) EagleDbSqlValue_NewWithInteger(atoi(EagleDbParser_LastToken()))));
    }
    |
    IDENTIFIER {
        EagleDbParser_AddReturn(EagleDbParser_AddObject((void*) EagleDbSqlValue_NewWithIdentifier(EagleDbParser_LastToken())));
    }
;

%%
