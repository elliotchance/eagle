%{

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define YYSTYPE int

%}

%error-verbose

%token NUMBER
%token PLUS MINUS TIMES DIVIDE POWER
%token LEFT RIGHT
%token END

%left PLUS MINUS
%left TIMES DIVIDE
%left NEG
%right POWER

%%

input:
    | input expression
;

expression:
    NUMBER PLUS NUMBER { $$ = $1 + $3; printf("%d\n", $$); }
;

%%

int yyerror(char *s)
{
    printf("Error: %s\n", s);
}
