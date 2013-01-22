#ifndef eagle_EagleDbParser_h
#define eagle_EagleDbParser_h

#define MAX_YYOBJ 256
#define MAX_YYERRORS 100
#define MAX_YYRETURN 256
#define MAX_YYLIST 256

/**
 Internal prototype provided by flex and bison.
 */
int yy_scan_string(const char *str);

/**
 Internal prototype provided by flex and bison.
 */
int yyparse();

int yyerror(char *s);

int yylex();

int yylex_destroy();

char* yyerrors_last();

void yylex_init();

void yylex_free();

char* yyerrors_push(void *ptr);

void* yyobj_push(void *ptr);

void* yylist_push(void *ptr);

void* yylist_new();

void* yyreturn_push(void *ptr);

extern char **yyerrors;

extern int yyerrors_length;

/**
 This is the pointer to the final AST returned by the parser.
 */
extern void *yyparse_ast;

extern void **yyobj;

extern int yyobj_length;

/**
 When returning an array of something you can use this mechanism.
 */
extern void **yylist;

extern int yylist_length;

/**
 A return stack. Maximum depth is 256.
 */
extern void **yyreturn;

extern int yyreturn_length;

#endif
