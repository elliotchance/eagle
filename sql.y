%{

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yyerror(char *s);
int yylex();

char *yyerror_last = NULL;
void *yyparse_ast = NULL;
extern char *yytext_last;

#include "EagleDbSqlSelect.h"

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
%token T_END

%%

input:
    | input statement
;

statement:
    select_statement
;

select_statement:
    K_SELECT {
        yyparse_ast = (void*) EagleDbSqlSelect_New();
    }
    T_ASTERISK K_FROM IDENTIFIER {
        ((EagleDbSqlSelect*) yyparse_ast)->tableName = strdup(yytext_last);
    }
    where_expression
;

where_expression:
    | K_WHERE expression {
        ((EagleDbSqlSelect*) yyparse_ast)->whereExpression = EagleDbSqlValue_NewWithInteger(atoi(yytext_last));
    }
;

expression:
    INTEGER

%%

int yyerror(char *s)
{
    yyerror_last = s;
    return 0;
}
