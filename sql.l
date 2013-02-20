%{
    
    #include <stdlib.h>
    #include "y.tab.h"
    #include "EagleDbParser.h"
    
    #define YYSTYPE void*
    #define YY_EXTRA_TYPE EagleDbParser*

%}

/* SQL standard is not case sensitive */
%option case-insensitive

/* this parser must be thread safe */
%option warn nodefault
%option reentrant noyywrap never-interactive nounistd
%option bison-bridge

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
"AND" return K_AND;
"OR" return K_OR;
"NOT" return K_NOT;

">=" return T_GREATER_THAN_EQUAL;
"<=" return T_LESS_THAN_EQUAL;
"!=" return T_NOT_EQUALS;
">" return T_GREATER_THAN;
"<" return T_LESS_THAN;
"*" return T_ASTERISK;
"+" return T_PLUS;
"-" return T_MINUS;
"/" return T_DIVIDE;
"%" return T_MODULUS;
"=" return T_EQUALS;
";" return T_END;
"," return T_COMMA;
"(" return T_BRACKET_OPEN;
")" return T_BRACKET_CLOSE;

{white} {
    /* skip white space */
}

{identifier} {
    yyextra->yytext_last = yytext;
    return IDENTIFIER;
}

{integer} {
    yyextra->yytext_last = yytext;
    return INTEGER;
}
