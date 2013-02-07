%{

#define YYSTYPE void*
#include "y.tab.h"
#include <stdlib.h>

char *yytext_last = NULL;

%}

%option case-insensitive

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
"TEXT" return K_TEXT;
"WHERE" return K_WHERE;
"VALUES" return K_VALUES;
"INSERT" return K_INSERT;
"INTO" return K_INTO;

">=" return T_GREATER_THAN_EQUAL;
"<=" return T_LESS_THAN_EQUAL;
"!=" return T_NOT_EQUALS;
">" return T_GREATER_THAN;
"<" return T_LESS_THAN;
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
