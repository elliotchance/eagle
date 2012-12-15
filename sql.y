%{

    #include <math.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #include "EagleDbSqlSelect.h"
    #include "EagleDbSqlBinaryExpression.h"

    int yyerror(char *s);
    int yylex();
    int yylex_destroy();

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
        yyreturn_push(yyobj_push((void*) EagleDbSqlSelect_New()));
    }
    T_ASTERISK
    K_FROM IDENTIFIER {
        ((EagleDbSqlSelect*) yyreturn_current())->tableName = yyobj_push(strdup(yytext_last));
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
            yyreturn_push(yyobj_push((void*) EagleDbSqlBinaryExpression_New((EagleDbSqlExpression*) last, 0, NULL)));
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
        yyreturn_push(yyobj_push((void*) EagleDbSqlValue_NewWithInteger(atoi(yytext_last))));
    }

%%

int yyerror(char *s)
{
    yyerrors_push(strdup(s));
    return 0;
}

void yylex_init()
{
    yyobj = (void**) calloc((size_t) 256, sizeof(void*));
    yyobj_length = 0;
    
    yyerrors = (char**) calloc((size_t) 100, sizeof(char*));
    yyerrors_length = 0;
    
    yyreturn = (void**) calloc((size_t) 256, sizeof(void*));
    yyreturn_length = 0;
}

void yylex_free()
{
    int i;
    
    /* yyobj */
    if(yyerrors_length > 0) {
        for(i = 0; i < yyerrors_length; ++i) {
            free(yyobj[i]);
        }
    }
    free(yyobj);
    yyobj = NULL;
    
    /* yyreturn */
    free(yyreturn);
    yyreturn = NULL;
    
    /* yyerrors */
    for(i = 0; i < yyerrors_length; ++i) {
        free(yyerrors[i]);
    }
    free(yyerrors);
    yyerrors = NULL;
    
    yylex_destroy();
}
