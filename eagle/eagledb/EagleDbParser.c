#include <stdlib.h>
#include "EagleDbParser.h"

char **yyerrors = NULL;
int yyerrors_length = 0;
void *yyparse_ast = NULL;
void **yyobj = NULL;
int yyobj_length = 0;
void **yylist = NULL;
int yylist_length = 0;
void **yyreturn = NULL;
int yyreturn_length = 0;
