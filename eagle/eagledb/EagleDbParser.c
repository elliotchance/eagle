#include <stdlib.h>
#include "EagleDbParser.h"

/**
 Contains the first 100 error messages.
 */
char **yyerrors = NULL;
int yyerrors_length = 0;

/**
 This is the pointer to the final AST returned by the parser.
 */
void *yyparse_ast = NULL;

void **yyobj = NULL;
int yyobj_length = 0;

/**
 When returning an array of something you can use this mechanism.
 */
void **yylist = NULL;
int yylist_length = 0;

/**
 A return stack. Maximum depth is 256.
 */
void **yyreturn = NULL;
int yyreturn_length = 0;
