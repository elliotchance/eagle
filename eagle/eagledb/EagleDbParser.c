#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Eagle.h"
#include "EagleDbParser.h"
#include "EagleDbSqlStatementType.h"
#include "EagleMemory.h"
#include "EagleData.h"

static EagleDbParser *EagleDbParser_Default = NULL;

EagleDbParser* EagleDbParser_New(void)
{
    EagleDbParser *parser = (EagleDbParser*) EagleMemory_Allocate("EagleDbParser_New.1", sizeof(EagleDbParser));
    if(NULL == parser) {
        return NULL;
    }
    
    parser->yyerrors = NULL;
    parser->yyerrors_length = 0;
    parser->yylist = NULL;
    parser->yylist_length = 0;
    parser->yyobj = NULL;
    parser->yyobj_length = 0;
    parser->yyreturn = NULL;
    parser->yyreturn_length = 0;
    parser->yyparse_ast = NULL;
    parser->yystatementtype = EagleDbSqlStatementTypeNone;
    
    return parser;
}

EagleDbParser* EagleDbParser_Get(void)
{
    return EagleDbParser_Default;
}

/**
 Push the error onto the stack.
 */
char* yyerrors_push(void *ptr)
{
    EagleDbParser *p = EagleDbParser_Default;
    
    /* we have reached the maximum amount of errors, in this case we just don't store any more errors */
    if(p->yyerrors_length >= MAX_YYERRORS) {
        EagleMemory_Free(ptr);
        return NULL;
    }
    
    return p->yyerrors[p->yyerrors_length++] = ptr;
}

/**
 Returns the most recent error message.
 */
char* yyerrors_last()
{
    EagleDbParser *p = EagleDbParser_Default;
    return p->yyerrors[p->yyerrors_length - 1];
}

/**
 Anything that is allocated during the parsing must call yyobj_push(), if the parsing fails this stack will do the cleanup.
 */
void* yyobj_push(void *ptr)
{
    EagleDbParser *p = EagleDbParser_Default;
    
    if(p->yyobj_length >= MAX_YYOBJ) {
        char *msg = (char*) EagleMemory_Allocate("yyobj_push.1", 64);
        if(NULL == msg) {
            return NULL;
        }
        
        sprintf(msg, "Cannot parse SQL. Maximum depth of %d exceeded.", MAX_YYOBJ);
        yyerrors_push(msg);
        return NULL;
    }
    
    return p->yyobj[p->yyobj_length++] = ptr;
}

void* yylist_push(void *ptr)
{
    EagleDbParser *p = EagleDbParser_Default;
    
    if(p->yylist_length >= MAX_YYLIST) {
        char *msg = (char*) EagleMemory_Allocate("yylist_push.1", 64);
        if(NULL == msg) {
            return NULL;
        }
        
        sprintf(msg, "Cannot parse SQL. Maximum list size of %d exceeded.", MAX_YYLIST);
        yyerrors_push(msg);
        return NULL;
    }
    
    return p->yylist[p->yylist_length++] = ptr;
}

void* yylist_new()
{
    EagleDbParser *p = EagleDbParser_Default;
    
    p->yylist = (void**) EagleMemory_MultiAllocate("yylist_new.1", sizeof(void*), MAX_YYLIST);
    p->yylist_length = 0;
    
    return p->yylist;
}

/**
 Push the return value onto the stack.
 */
void* yyreturn_push(void *ptr)
{
    EagleDbParser *p = EagleDbParser_Default;
    
    if(p->yyreturn_length >= MAX_YYRETURN) {
        char *msg = (char*) EagleMemory_Allocate("yyreturn_push.1", 64);
        if(NULL == msg) {
            return NULL;
        }
        
        sprintf(msg, "Cannot parse SQL. Maximum return depth of %d exceeded.", MAX_YYRETURN);
        yyerrors_push(msg);
        return NULL;
    }
    
    return p->yyreturn[p->yyreturn_length++] = ptr;
}

/**
 Return the last yyreturn and decrement back the stack.
 */
void* yyreturn_pop()
{
    EagleDbParser *p = EagleDbParser_Default;
    return p->yyreturn[--p->yyreturn_length];
}

/**
 Return the most recent yyreturn.
 */
void* yyreturn_current()
{
    EagleDbParser *p = EagleDbParser_Default;
    return p->yyreturn[p->yyreturn_length - 1];
}

int yyerror(char *s)
{
    yyerrors_push(strdup(s));
    return 0;
}

void yylex_init()
{
    EagleDbParser *p;
    p = EagleDbParser_Default = EagleDbParser_New();
    
    p->yyobj = (void**) EagleMemory_MultiAllocate("yylex_init.1", sizeof(void*), MAX_YYOBJ);
    p->yyobj_length = 0;
    
    p->yyerrors = (char**) EagleMemory_MultiAllocate("yylex_init.2", sizeof(char*), MAX_YYERRORS);
    p->yyerrors_length = 0;
    
    p->yyreturn = (void**) EagleMemory_MultiAllocate("yylex_init.3", sizeof(void*), MAX_YYRETURN);
    p->yyreturn_length = 0;
}

void yylex_free()
{
    int i;
    EagleDbParser *p = EagleDbParser_Default;
    
    /* yyobj */
    if(p->yyerrors_length > 0) {
        for(i = 0; i < p->yyobj_length; ++i) {
            EagleMemory_Free(p->yyobj[i]);
        }
    }
    EagleMemory_Free(p->yyobj);
    p->yyobj = NULL;
    
    /* yyreturn */
    EagleMemory_Free(p->yyreturn);
    p->yyreturn = NULL;
    
    /* yyerrors */
    for(i = 0; i < p->yyerrors_length; ++i) {
        EagleMemory_Free(p->yyerrors[i]);
    }
    EagleMemory_Free(p->yyerrors);
    p->yyerrors = NULL;
    
    EagleMemory_Free(p);
    yylex_destroy();
}
