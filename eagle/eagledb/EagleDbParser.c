#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Eagle.h"
#include "EagleDbParser.h"
#include "EagleDbSqlStatementType.h"
#include "EagleMemory.h"
#include "EagleData.h"

char **yyerrors = NULL;
int yyerrors_length = 0;
void *yyparse_ast = NULL;
void **yyobj = NULL;
int yyobj_length = 0;
void **yylist = NULL;
int yylist_length = 0;
void **yyreturn = NULL;
int yyreturn_length = 0;

EagleDbSqlStatementType yystatementtype = EagleDbSqlStatementTypeNone;

/**
 Push the error onto the stack.
 */
char* yyerrors_push(void *ptr)
{
    /* we have reached the maximum amount of errors, in this case we just don't store any more errors */
    if(yyerrors_length >= MAX_YYERRORS) {
        free(ptr);
        return NULL;
    }
    return yyerrors[yyerrors_length++] = ptr;
}

/**
 Returns the most recent error message.
 */
char* yyerrors_last()
{
    return yyerrors[yyerrors_length - 1];
}

/**
 Anything that is allocated during the parsing must call yyobj_push(), if the parsing fails this stack will do the cleanup.
 */
void* yyobj_push(void *ptr)
{
    if(yyobj_length >= MAX_YYOBJ) {
        char *msg = (char*) malloc(64);
        sprintf(msg, "Cannot parse SQL. Maximum depth of %d exceeded.", MAX_YYOBJ);
        yyerrors_push(msg);
        return NULL;
    }
    return yyobj[yyobj_length++] = ptr;
}

void* yylist_push(void *ptr)
{
    if(yylist_length >= MAX_YYLIST) {
        char *msg = (char*) malloc(64);
        sprintf(msg, "Cannot parse SQL. Maximum list size of %d exceeded.", MAX_YYLIST);
        yyerrors_push(msg);
        return NULL;
    }
    return yylist[yylist_length++] = ptr;
}

void* yylist_new()
{
    yylist = (void**) EagleMemory_MultiAllocate("yylist_new.1", sizeof(void*), MAX_YYLIST);
    yylist_length = 0;
    return yylist;
}

/**
 Push the return value onto the stack.
 */
void* yyreturn_push(void *ptr)
{
    if(yyreturn_length >= MAX_YYRETURN) {
        char *msg = (char*) malloc(64);
        sprintf(msg, "Cannot parse SQL. Maximum return depth of %d exceeded.", MAX_YYRETURN);
        yyerrors_push(msg);
        return NULL;
    }
    return yyreturn[yyreturn_length++] = ptr;
}

/**
 Return the last yyreturn and decrement back the stack.
 */
void* yyreturn_pop()
{
    return yyreturn[--yyreturn_length];
}

/**
 Return the most recent yyreturn.
 */
void* yyreturn_current()
{
    return yyreturn[yyreturn_length - 1];
}

EagleDataType yy_data_type;

int yyerror(char *s)
{
    yyerrors_push(strdup(s));
    return 0;
}

void yylex_init()
{
    yyobj = (void**) EagleMemory_MultiAllocate("yylex_init.1", sizeof(void*), MAX_YYOBJ);
    yyobj_length = 0;
    
    yyerrors = (char**) EagleMemory_MultiAllocate("yylex_init.2", sizeof(char*), MAX_YYERRORS);
    yyerrors_length = 0;
    
    yyreturn = (void**) EagleMemory_MultiAllocate("yylex_init.3", sizeof(void*), MAX_YYRETURN);
    yyreturn_length = 0;
}

void yylex_free()
{
    int i;
    
    /* yyobj */
    if(yyerrors_length > 0) {
        for(i = 0; i < yyobj_length; ++i) {
            EagleMemory_Free(yyobj[i]);
        }
    }
    EagleMemory_Free(yyobj);
    yyobj = NULL;
    
    /* yyreturn */
    EagleMemory_Free(yyreturn);
    yyreturn = NULL;
    
    /* yyerrors */
    for(i = 0; i < yyerrors_length; ++i) {
        EagleMemory_Free(yyerrors[i]);
    }
    EagleMemory_Free(yyerrors);
    yyerrors = NULL;
    
    yylex_destroy();
}
