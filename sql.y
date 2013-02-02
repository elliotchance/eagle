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
    #include "EagleDbSqlInsert.h"
    
    int yylex(void);
    
    #define ADD_ERROR(fmt, ...) { \
    char msg[1024]; \
    sprintf(msg, fmt, __VA_ARGS__); \
    yyerror(msg); \
    }
    
    #define RAISE_ERROR(fmt, ...) { \
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
%token T_ASTERISK
%token T_PLUS
%token T_EQUALS
%token T_END ";"
%token T_COMMA
%token T_BRACKET_OPEN
%token T_BRACKET_CLOSE

%token END 0 "end of file"

%%

/*
 ( statement? T_END )? END
 */
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
    statement {
        EagleDbParser *p = EagleDbParser_Get();
        p->yyparse_ast = EagleDbParser_PopReturn();
    }
    END
    |
    statement {
        EagleDbParser *p = EagleDbParser_Get();
        p->yyparse_ast = EagleDbParser_PopReturn();
    }
    T_END END
;

/*
 select_statement | create_table_statement
 */
statement:
    select_statement {
        EagleDbParser *p = EagleDbParser_Get();
        p->yystatementtype = EagleDbSqlStatementTypeSelect;
    }
    |
    create_table_statement {
        EagleDbParser *p = EagleDbParser_Get();
        p->yystatementtype = EagleDbSqlStatementTypeCreateTable;
    }
    |
    insert_statement {
        EagleDbParser *p = EagleDbParser_Get();
        p->yystatementtype = EagleDbSqlStatementTypeInsert;
    }
;

/*
 K_INSERT K_INTO IDENTIFIER T_BRACKET_OPEN column_expression_list T_BRACKET_CLOSE
 K_VALUES T_BRACKET_OPEN column_expression_list T_BRACKET_CLOSE
 @return EagleDbSqlInsert
 */
insert_statement:
    K_INSERT K_INTO IDENTIFIER {
        EagleDbSqlInsert *insert = EagleDbSqlInsert_New();
        insert->table = strdup(EagleDbParser_LastToken());
        EagleDbParser_AddReturn(insert, (void(*)(void*)) EagleDbSqlInsert_Delete);
    }
    T_BRACKET_OPEN
    column_expression_list {
        EagleLinkedList *last = EagleDbParser_PopReturn();
        ((EagleDbSqlInsert*) EagleDbParser_CurrentReturn())->names = last;
    }
    T_BRACKET_CLOSE
    K_VALUES
    T_BRACKET_OPEN
    column_expression_list {
        EagleLinkedList *last = EagleDbParser_PopReturn();
        ((EagleDbSqlInsert*) EagleDbParser_CurrentReturn())->values = last;
    }
    T_BRACKET_CLOSE
;

/*
 K_CREATE K_TABLE IDENTIFIER T_BRACKET_OPEN column_definition_list T_BRACKET_CLOSE
 @return EagleDbTable
 */
create_table_statement:
    K_CREATE K_TABLE IDENTIFIER {
        EagleDbTable *table = EagleDbTable_New(EagleDbParser_LastToken());
        EagleDbParser_AddReturn(table, (void(*)(void*)) EagleDbTable_Delete);
    }
    T_BRACKET_OPEN
    column_definition_list {
        EagleLinkedList *last = EagleDbParser_PopReturn();
        EagleDbTable_setColumns((EagleDbTable*) EagleDbParser_CurrentReturn(), last);
    }
    T_BRACKET_CLOSE
;

/*
 column_definition next_column_definition
 @return EagleLinkedList
 */
column_definition_list:
    {
        EagleLinkedList *list = EagleLinkedList_New(); 
        EagleDbParser_AddReturn(list, (void(*)(void*)) EagleLinkedList_Delete);
    }
    column_definition {
        EagleDbColumn *last = (EagleDbColumn*) EagleDbParser_PopReturn();
        EagleLinkedListItem *item = EagleLinkedListItem_New(last, EagleTrue, (void(*)(void*)) EagleDbColumn_Delete);
        EagleLinkedList_add((EagleLinkedList*) EagleDbParser_CurrentReturn(), item);
    }
    next_column_definition
;

/*
 IDENTIFIER data_type
 @return EagleDbSqlColumn
 */
column_definition:
    IDENTIFIER {
        EagleDbColumn *column = EagleDbColumn_New(EagleDbParser_LastToken(), EagleDataTypeInteger);
        EagleDbParser_AddReturn(column, (void(*)(void*)) EagleDbColumn_Delete);
    }
    data_type {
        int *last = EagleDbParser_PopReturn();
        ((EagleDbColumn*) EagleDbParser_CurrentReturn())->type = *last;
        EagleMemory_Free(last);
    }
;

/*
 K_INTEGER | K_TEXT
 @return int*
 */
data_type:
    K_INTEGER {
        EagleDbParser_AddReturn(EagleData_Int(EagleDataTypeInteger), NULL);
    }
    |
    K_TEXT {
        EagleDbParser_AddReturn(EagleData_Int(EagleDataTypeText), NULL);
    }
;

/*
 ( T_COMMA column_definition next_column_definition )?
 @return EagleLinkedListItem
 */
next_column_definition:
    |
    T_COMMA column_definition {
        EagleDbColumn *last = EagleDbParser_PopReturn();
        EagleLinkedListItem *item = EagleLinkedListItem_New(last, EagleTrue, (void(*)(void*)) EagleDbColumn_Delete);
        EagleLinkedList_add((EagleLinkedList*) EagleDbParser_CurrentReturn(), item);
    }
    next_column_definition
;

keyword:
    K_CREATE | K_FROM | K_INTEGER | K_SELECT | K_TABLE | K_TEXT | K_WHERE | K_VALUES | K_INSERT | K_INTO
;

table_name:
    error {
        ADD_ERROR("%s", "Expected table name");
        EagleDbParser_AddReturn(NULL, NULL);
    }
    |
    keyword {
        ADD_ERROR("You cannot use the keyword \"%s\" for a table name.", EagleDbParser_LastToken());
        EagleDbParser_AddReturn(NULL, NULL);
    }
    |
    IDENTIFIER {
        EagleDbParser_AddReturn(strdup(EagleDbParser_LastToken()), NULL);
    }
;

/*
 K_SELECT column_expression_list K_FROM IDENTIFIER where_expression
 @return EagleDbSqlSelect
 */
select_statement:
    K_SELECT column_expression_list {
        EagleDbParser *p = EagleDbParser_Get();
        p->yystatementtype = EagleDbSqlStatementTypeSelect;
        
        EagleLinkedList *last = (EagleLinkedList*) EagleDbParser_PopReturn();
        EagleLinkedList_DeleteWithItems(last);
        RAISE_ERROR("%s", "Expected FROM after column list.");
    }
    error
    |
    K_SELECT error {
        EagleDbParser *p = EagleDbParser_Get();
        p->yystatementtype = EagleDbSqlStatementTypeSelect;
        RAISE_ERROR("%s", "Expected column list after SELECT.");
    }
    |
    K_SELECT column_expression_list {
        EagleDbSqlSelect *select = EagleDbSqlSelect_New();
        select->selectExpressions = EagleDbParser_PopReturn();
        EagleDbParser_AddReturn(select, (void(*)(void*)) EagleDbSqlSelect_Delete);
    }
    K_FROM table_name {
        char *last = EagleDbParser_PopReturn();
        ((EagleDbSqlSelect*) EagleDbParser_CurrentReturn())->tableName = last;
    }
    where_expression {
        void *last = EagleDbParser_PopReturn();
        ((EagleDbSqlSelect*) EagleDbParser_CurrentReturn())->whereExpression = last;
    }
;

/*
 column_expression next_column_expression
 @return EagleLinkedList
 */
column_expression_list:
    column_expression {
        EagleLinkedList *list = EagleLinkedList_New();
        EagleLinkedListItem *last = EagleLinkedListItem_New(EagleDbParser_PopReturn(), EagleTrue, (void(*)(void*)) EagleDbSqlExpression_DeleteRecursive);
        EagleLinkedList_add(list, last);
        EagleDbParser_AddReturn(list, (void(*)(void*)) EagleLinkedList_DeleteWithItems);
    }
    next_column_expression
;

/*
 T_ASTERISK | expression
 @return EagleDbSqlExpression
 */
column_expression:
    T_ASTERISK {
        void *last = EagleDbSqlValue_NewWithAsterisk();
        EagleDbParser_AddReturn(last, (void(*)(void*)) EagleDbSqlValue_Delete);
    }
    |
    expression {
        void *last = EagleDbParser_PopReturn();
        EagleDbParser_AddReturn(last, (void(*)(void*)) EagleDbSqlValue_Delete);
    }
;

/*
 ( T_COMMA column_expression next_column_expression )?
 @return none
 */
next_column_expression:
    |
    T_COMMA column_expression {
        EagleLinkedListItem *last = EagleLinkedListItem_New(EagleDbParser_PopReturn(), EagleTrue, (void(*)(void*)) EagleDbSqlExpression_DeleteRecursive);
        EagleLinkedList_add((EagleLinkedList*) EagleDbParser_CurrentReturn(), last);
    }
    next_column_expression
;

/*
 ( K_WHERE expression )?
 @return EagleDbSqlExpression | NULL
 */
where_expression:
    {
        EagleDbParser_AddReturn(NULL, NULL);
    }
    |
    K_WHERE expression
;

binary_expression_right:
    operator error {
        EagleDbSqlExpression *current = (EagleDbSqlExpression*) EagleDbParser_PopReturn();
        EagleDbSqlExpression_Delete(current);
    }
    |
    operator {
        int *last = EagleDbParser_PopReturn();
        ((EagleDbSqlBinaryExpression*) EagleDbParser_CurrentReturn())->op = *last;
        EagleMemory_Free(last);
    }
    value {
        EagleDbSqlExpression *last = EagleDbParser_PopReturn();
        ((EagleDbSqlBinaryExpression*) EagleDbParser_CurrentReturn())->right = last;
    }
;

binary_expression:
    value {
        EagleDbSqlExpression *last = EagleDbParser_PopReturn();
        EagleDbSqlBinaryExpression *expr = EagleDbSqlBinaryExpression_New(last, 0, NULL);
        EagleDbParser_AddReturn(expr, (void(*)(void*)) EagleDbSqlExpression_Delete);
    }
    binary_expression_right
;

/*
 @return EagleDbSqlExpression
 */
expression:
    value
    |
    binary_expression
;

/**
 T_PLUS | T_EQUALS
 @return int*
 */
operator:
    T_PLUS {
        EagleDbParser_AddReturn(EagleData_Int(EagleDbSqlExpressionOperatorPlus), NULL);
    }
    |
    T_EQUALS {
        EagleDbParser_AddReturn(EagleData_Int(EagleDbSqlExpressionOperatorEquals), NULL);
    }
;

/*
 INTEGER | IDENTIFIER
 @return EagleDbSqlValue
 */
value:
    INTEGER {
        void *v = EagleDbSqlValue_NewWithInteger(atoi(EagleDbParser_LastToken()));
        EagleDbParser_AddReturn(v, (void(*)(void*)) EagleDbSqlValue_Delete);
    }
    |
    IDENTIFIER {
        void *v = EagleDbSqlValue_NewWithIdentifier(EagleDbParser_LastToken());
        EagleDbParser_AddReturn(v, (void(*)(void*)) EagleDbSqlValue_Delete);
    }
;

%%
