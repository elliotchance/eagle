%{
#define YYSTYPE int
#include "sql.tab.h"
#include <stdlib.h>
%}

white [ \t]+
digit [0-9]
integer {digit}+

%%

{white} { }
{integer} {
    yylval = atoi(yytext);
    printf("real = '%s' %d\n", yytext, yylval);
    return NUMBER;
}

"+" return PLUS;
"-" return MINUS;
"*" return TIMES;
"/" return DIVIDE;
"^" return POWER;
"(" return LEFT;
")" return RIGHT;
"\n" return END;
