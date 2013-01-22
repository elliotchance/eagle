#ifndef eagle_EagleDbParser_h
#define eagle_EagleDbParser_h

#define MAX_YYOBJ 256
#define MAX_YYERRORS 100
#define MAX_YYRETURN 256
#define MAX_YYLIST 256

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
extern void *yyparse_ast;
extern void **yyobj;
extern int yyobj_length;
extern void **yylist;
extern int yylist_length;
extern void **yyreturn;
extern int yyreturn_length;

#endif
