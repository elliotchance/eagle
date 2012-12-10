%{

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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

/* variable tokens */
%token IDENTIFIER

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
        ((EagleDbSqlSelect*) yyparse_ast)->tableName = yytext_last;
    }
;

%%

int yyerror(char *s)
{
    yyerror_last = s;
    return 0;
}
