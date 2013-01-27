#ifndef eagle_EagleDbParser_h
#define eagle_EagleDbParser_h

#include "EagleData.h"

#define MAX_YYOBJ 256
#define MAX_YYERRORS 100
#define MAX_YYRETURN 256
#define MAX_YYLIST 256

/**
 Internal prototype provided by flex and bison.
 */
int yy_scan_string(const char *str);

/**
 This can be used to get the most recent yytext token. This is not a data duplication of the token so you must copy it
 out if you intended to keep it. Since you cannot access the yytext direct from here?
 */
extern char *yytext_last;

/**
 Internal prototype provided by flex and bison.
 */
int yyparse(void);

/**
 Used by "data_type"
 */
extern EagleDataType yy_data_type;

int yyerror(char *s);

int yylex(void);

int yylex_destroy(void);

char* yyerrors_last(void);

void yylex_init(void);

void yylex_free(void);

char* yyerrors_push(void *ptr);

void* yyobj_push(void *ptr);

void* yylist_push(void *ptr);

void* yylist_new(void);

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

void* yyreturn_pop(void);

void* yyreturn_current(void);

#endif
