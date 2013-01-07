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

    int yyerror(char *s);
    int yylex();
    int yylex_destroy();
    
    EagleDbSqlStatementType yystatementtype = EagleDbSqlStatementTypeNone;

    /**
     Contains the first 100 error messages.
     */
    char **yyerrors = NULL;
    int yyerrors_length = 0;
    
    /**
     Push the error onto the stack.
     */
    char* yyerrors_push(void *ptr)
    {
        return yyerrors[yyerrors_length++] = ptr;
    }
    
    /**
     Returns the most recent error message.
     */
    char* yyerrors_last()
    {
        return yyerrors[yyerrors_length - 1];
    }

    /**
     This is the pointer to the final AST returned by the parser.
     */
    void *yyparse_ast = NULL;
    
    void **yyobj = NULL;
    int yyobj_length = 0;
    
    /**
     Anything that is allocated during the parsing must call yyobj_push(), if the parsing fails this stack will do the cleanup.
     */
    void* yyobj_push(void *ptr)
    {
        return yyobj[yyobj_length++] = ptr;
    }
    
    /**
     When returning an array of something you can use this mechanism.
     */
    void **yylist = NULL;
    int yylist_length = 0;
    
    void* yylist_push(void *ptr)
    {
        return yylist[yylist_length++] = ptr;
    }
    
    void* yylist_new()
    {
        yylist = (void**) EagleMemory_MultiAllocate("yylist_new.1", sizeof(void*), 256);
        yylist_length = 0;
        return yylist;
    }

    /**
     A return stack. Maximum depth is 256.
     */
    void **yyreturn = NULL;
    int yyreturn_length = 0;

    /**
     Push the return value onto the stack.
     */
    void* yyreturn_push(void *ptr)
    {
        return yyreturn[yyreturn_length++] = ptr;
    }

    /**
     Return the last yyreturn and decrement back the stack.
     */
    void* yyreturn_pop()
    {
        return yyreturn[--yyreturn_length];
    }

    /**
     Return the most recent yyreturn.
     */
    void* yyreturn_current()
    {
        return yyreturn[yyreturn_length - 1];
    }

    /**
     This can be used to get the most recent yytext token. This is not a data duplication of the token so you must copy it
     out if you intended to keep it. Since you cannot access the yytext direct from here?
     */
    extern char *yytext_last;
    
    /**
     Used by "data_type"
     */
    EagleDataType yy_data_type;

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
        yyparse_ast = NULL;
        yystatementtype = EagleDbSqlStatementTypeNone;
    }
    |
    T_END {
        yyparse_ast = NULL;
        yystatementtype = EagleDbSqlStatementTypeNone;
    }
    |
    statement {
        yyparse_ast = yyreturn_pop();
    }
    T_END
;

/*
 select_statement | create_table_statement
 */
statement:
    select_statement {
        /* bubble up yyreturn */
        yystatementtype = EagleDbSqlStatementTypeSelect;
    }
    |
    create_table_statement {
        /* bubble up yyreturn */
        yystatementtype = EagleDbSqlStatementTypeCreateTable;
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
        void *last = yyreturn_pop();
        EagleDbTable_setColumns((EagleDbTable*) yyreturn_current(), last, yylist_length);
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
        yyreturn_push(yylist);
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
        ((EagleDbColumn*) yyreturn_current())->type = yy_data_type;
    }
;

/*
 K_INTEGER | K_TEXT
 */
data_type:
    K_INTEGER {
        yy_data_type = EagleDataTypeInteger;
    }
    |
    K_TEXT {
        yy_data_type = EagleDataTypeText;
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
        void *last = yyreturn_pop();
        ((EagleDbSqlSelect*) yyreturn_current())->selectExpressions = last;
        ((EagleDbSqlSelect*) yyreturn_current())->allocatedSelectExpressions = yylist_length;
        ((EagleDbSqlSelect*) yyreturn_current())->usedSelectExpressions = yylist_length;
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
        yyreturn_push(yylist);
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

int yyerror(char *s)
{
    yyerrors_push(strdup(s));
    return 0;
}

void yylex_init()
{
    yyobj = (void**) EagleMemory_MultiAllocate("yylex_init.1", sizeof(void*), 256);
    yyobj_length = 0;
    
    yyerrors = (char**) EagleMemory_MultiAllocate("yylex_init.2", sizeof(char*), 100);
    yyerrors_length = 0;
    
    yyreturn = (void**) EagleMemory_MultiAllocate("yylex_init.3", sizeof(void*), 256);
    yyreturn_length = 0;
}

void yylex_free()
{
    int i;
    
    /* yyobj */
    if(yyerrors_length > 0) {
        for(i = 0; i < yyobj_length; ++i) {
            EagleMemory_Free(yyobj[i]);
        }
    }
    EagleMemory_Free(yyobj);
    yyobj = NULL;
    
    /* yyreturn */
    EagleMemory_Free(yyreturn);
    yyreturn = NULL;
    
    /* yyerrors */
    for(i = 0; i < yyerrors_length; ++i) {
        EagleMemory_Free(yyerrors[i]);
    }
    EagleMemory_Free(yyerrors);
    yyerrors = NULL;
    
    yylex_destroy();
}
