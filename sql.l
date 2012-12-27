%{

#include "y.tab.h"
#include <stdlib.h>

char *yytext_last = NULL;

%}

white [ \t\n\r]+
identifier [a-zA-Z_][a-zA-Z0-9_]*
integer [0-9]+

%%

"CREATE" return K_CREATE;
"FROM" return K_FROM;
"INT" return K_INTEGER;
"INTEGER" return K_INTEGER;
"SELECT" return K_SELECT;
"TABLE" return K_TABLE;
"WHERE" return K_WHERE;

"*" return T_ASTERISK;
"+" return T_PLUS;
"=" return T_EQUALS;
";" return T_END;
"," return T_COMMA;
"(" return T_BRACKET_OPEN;
")" return T_BRACKET_CLOSE;

{white} {
    /* skip white space */
}

{identifier} {
    yytext_last = yytext;
    return IDENTIFIER;
}

{integer} {
    yytext_last = yytext;
    return INTEGER;
}
