%{

#include "y.tab.h"
#include <stdlib.h>

char *yytext_last = NULL;

%}

white [ \t\n\r]+
identifier [a-zA-Z_][a-zA-Z0-9_]*
integer [0-9]+

%%

"SELECT" return K_SELECT;
"FROM" return K_FROM;
"WHERE" return K_WHERE;

"*" return T_ASTERISK;
"+" return T_PLUS;
";" return T_END;

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
