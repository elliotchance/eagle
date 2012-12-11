%{

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "EagleDbSqlSelect.h"
#include "EagleDbSqlBinaryExpression.h"

int yyerror(char *s);
int yylex();

/**
 Contains the last error message. A dusplicate copy of the error message will always be taken so this actually leaks.
 It would be nice to add it to an error stack that can be free'd in bulk.
 */
char *yyerror_last = NULL;

/**
 This is the pointer to the final AST returned by the parser.
 */
void *yyparse_ast = NULL;

/**
 A return stack. Maximum depth is 256.
 */
void **yyreturn = NULL;
int yyreturn_length = 0;

/**
 Initialise the yyreturn stack.
 */
void** yyreturn_init()
{
    if(NULL == yyreturn) {
        yyreturn = (void**) calloc((size_t) 256, sizeof(void*));
    }
    return yyreturn;
}

/**
 Push the return value onto the stack.
 */
void* yyreturn_push(void *ptr)
{
    yyreturn_init();
    return yyreturn[yyreturn_length++] = ptr;
}

/**
 Return the last yyreturn and decrement back the stack.
 */
void* yyreturn_pop()
{
    yyreturn_init();
    return yyreturn[--yyreturn_length];
}

/**
 Return the most recent yyreturn.
 */
void* yyreturn_current()
{
    yyreturn_init();
    return yyreturn[yyreturn_length - 1];
}

/**
 This can be used to get the most recent yytext token. This is not a data duplication of the token so you must copy it
 out if you intended to keep it. Since you cannot access the yytext direct from here?
 */
extern char *yytext_last;

%}

/* be more versbose about error messages */
%error-verbose

/* keywords */
%token K_FROM
%token K_SELECT
%token K_WHERE

/* variable tokens */
%token IDENTIFIER
%token INTEGER

/* fixed tokens */
%token T_ASTERISK
%token T_PLUS
%token T_END

%%

input:
    {
        yyparse_ast = NULL;
    }
    |
        input statement {
            yyparse_ast = yyreturn_pop();
        }
;

statement:
    select_statement {
        /* bubble up yyreturn */
    }
;

select_statement:
    K_SELECT {
        yyreturn_push((void*) EagleDbSqlSelect_New());
    }
    T_ASTERISK
    K_FROM IDENTIFIER {
        ((EagleDbSqlSelect*) yyreturn_current())->tableName = strdup(yytext_last);
    }
    where_expression {
        void *last = yyreturn_pop();
        ((EagleDbSqlSelect*) yyreturn_current())->whereExpression = last;
    }
;

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

expression:
    value {
        /* bubble up yyreturn */
    }
    |
        value {
            void *last = yyreturn_pop();
            yyreturn_push((void*) EagleDbSqlBinaryExpression_New((EagleDbSqlExpression*) last, 0, NULL));
        }
        T_PLUS {
            ((EagleDbSqlBinaryExpression*) yyreturn_current())->op = EagleDbSqlExpressionOperatorPlus;
        }
        value {
            void *last = yyreturn_pop();
            ((EagleDbSqlBinaryExpression*) yyreturn_current())->right = (EagleDbSqlExpression*) last;
        }
    
value:
    INTEGER {
        yyreturn_push((void*) EagleDbSqlValue_NewWithInteger(atoi(yytext_last)));
    }

%%

int yyerror(char *s)
{
    yyerror_last = strdup(s);
    return 0;
}
