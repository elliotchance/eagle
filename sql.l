%{

#include "y.tab.h"
#include <stdlib.h>

char *yytext_last = NULL;

%}

white [ \t\n\r]+
identifier [a-zA-Z_][a-zA-Z0-9_]*

%%

"SELECT" return K_SELECT;
"FROM" return K_FROM;

"*" return T_ASTERISK;
";" return T_END;

{white} {
    /* skip white space */
}

{identifier} {
    yytext_last = yytext;
    return IDENTIFIER;
}
