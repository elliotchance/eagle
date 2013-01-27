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
 @return EagleDbTable
 */
create_table_statement:
    K_CREATE K_TABLE IDENTIFIER {
        EagleDbTable *table = EagleDbTable_New(EagleDbParser_LastToken());
        EagleDbParser_AddReturn(table);
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
        EagleDbParser_AddReturn(EagleLinkedList_New());
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
        EagleDbParser_AddReturn(column);
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
        EagleDbParser_AddReturn(EagleData_Int(EagleDataTypeInteger));
    }
    |
    K_TEXT {
        EagleDbParser_AddReturn(EagleData_Int(EagleDataTypeText));
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

/*
 K_SELECT column_expression_list K_FROM IDENTIFIER where_expression
 @return EagleDbSqlSelect
 */
select_statement:
    K_SELECT column_expression_list {
        EagleDbSqlSelect *select = EagleDbSqlSelect_New();
        select->selectExpressions = EagleDbParser_PopReturn();
        EagleDbParser_AddReturn(select);
    }
    K_FROM IDENTIFIER {
        ((EagleDbSqlSelect*) EagleDbParser_CurrentReturn())->tableName = strdup(EagleDbParser_LastToken());
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
        //EagleDbParser_AddObject(list, (void(*)(void*)) NULL);

        EagleLinkedListItem *last = EagleLinkedListItem_New(EagleDbParser_PopReturn(), EagleTrue, (void(*)(void*)) EagleDbSqlExpression_DeleteRecursive);
        EagleLinkedList_add(list, last);
        EagleDbParser_AddReturn(list);
    }
    next_column_expression
;

/*
 T_ASTERISK | expression
 @return EagleLinkedListItem
 */
column_expression:
    T_ASTERISK {
        void *last = (void*) EagleDbSqlValue_NewWithAsterisk();
        EagleDbParser_AddReturn(last);
    }
    |
    expression {
        void *last = EagleDbParser_PopReturn();
        EagleDbParser_AddReturn(last);
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
        /* no where clause */
        EagleDbParser_AddReturn(NULL);
    }
    |
    K_WHERE expression {
        /* bubble up expression */
    }
;

/*
 value ( ( T_PLUS | T_EQUALS ) value )?
 @return EagleDbSqlExpression
 */
expression:
    value {
        /* bubble up return */
    }
    |
    value {
        EagleDbSqlExpression *last = EagleDbParser_PopReturn();
        EagleDbParser_AddReturn((void*) EagleDbSqlBinaryExpression_New(last, 0, NULL));
    }
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

/**
 T_PLUS | T_EQUALS
 @return int*
 */
operator:
    T_PLUS {
        EagleDbParser_AddReturn(EagleData_Int(EagleDbSqlExpressionOperatorPlus));
    }
    |
    T_EQUALS {
        EagleDbParser_AddReturn(EagleData_Int(EagleDbSqlExpressionOperatorEquals));
    }
;

/*
 INTEGER | IDENTIFIER
 @return EagleDbSqlValue
 */
value:
    INTEGER {
        EagleDbParser_AddReturn(EagleDbSqlValue_NewWithInteger(atoi(EagleDbParser_LastToken())));
    }
    |
    IDENTIFIER {
        EagleDbParser_AddReturn(EagleDbSqlValue_NewWithIdentifier(EagleDbParser_LastToken()));
    }
;

%%
