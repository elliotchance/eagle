#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Eagle.h"
#include "EagleDbParser.h"
#include "EagleDbSqlStatementType.h"
#include "EagleMemory.h"
#include "EagleData.h"

static EagleDbParser *EagleDbParser_Default = NULL;

/* internal prototypes supplied by flex and bison */
extern int yylex_destroy(void);
extern int yy_scan_string(const char *str);
extern char *yytext_last;
extern int yyparse();

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

char* EagleDbParser_LastToken(void)
{
    return yytext_last;
}

EagleDbParser* EagleDbParser_Get(void)
{
    return EagleDbParser_Default;
}

/**
 Push the error onto the stack.
 */
char* EagleDbParser_AddError(void *ptr)
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
char* EagleDbParser_LastError()
{
    EagleDbParser *p = EagleDbParser_Default;
    return p->yyerrors[p->yyerrors_length - 1];
}

/**
 Anything that is allocated during the parsing must call EagleDbParser_AddObject(), if the parsing fails this stack will
 do the cleanup.
 */
void* EagleDbParser_AddObject(void *ptr)
{
    EagleDbParser *p = EagleDbParser_Default;
    
    if(p->yyobj_length >= MAX_YYOBJ) {
        char *msg = (char*) EagleMemory_Allocate("EagleDbParser_AddObject.1", 64);
        if(NULL == msg) {
            return NULL;
        }
        
        sprintf(msg, "Cannot parse SQL. Maximum depth of %d exceeded.", MAX_YYOBJ);
        EagleDbParser_AddError(msg);
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
        EagleDbParser_AddError(msg);
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
void* EagleDbParser_AddReturn(void *ptr)
{
    EagleDbParser *p = EagleDbParser_Default;
    
    if(p->yyreturn_length >= MAX_YYRETURN) {
        char *msg = (char*) EagleMemory_Allocate("EagleDbParser_AddReturn.1", 64);
        if(NULL == msg) {
            return NULL;
        }
        
        sprintf(msg, "Cannot parse SQL. Maximum return depth of %d exceeded.", MAX_YYRETURN);
        EagleDbParser_AddError(msg);
        return NULL;
    }
    
    return p->yyreturn[p->yyreturn_length++] = ptr;
}

/**
 Return the last yyreturn and decrement back the stack.
 */
void* EagleDbParser_PopReturn()
{
    EagleDbParser *p = EagleDbParser_Default;
    return p->yyreturn[--p->yyreturn_length];
}

/**
 Return the most recent yyreturn.
 */
void* EagleDbParser_CurrentReturn()
{
    EagleDbParser *p = EagleDbParser_Default;
    return p->yyreturn[p->yyreturn_length - 1];
}

int yyerror(char *s)
{
    EagleDbParser_AddError(strdup(s));
    return 0;
}

void EagleDbParser_Init()
{
    EagleDbParser *p;
    p = EagleDbParser_Default = EagleDbParser_New();
    
    p->yyobj = (void**) EagleMemory_MultiAllocate("EagleDbParser_Init.1", sizeof(void*), MAX_YYOBJ);
    p->yyobj_length = 0;
    
    p->yyerrors = (char**) EagleMemory_MultiAllocate("EagleDbParser_Init.2", sizeof(char*), MAX_YYERRORS);
    p->yyerrors_length = 0;
    
    p->yyreturn = (void**) EagleMemory_MultiAllocate("EagleDbParser_Init.3", sizeof(void*), MAX_YYRETURN);
    p->yyreturn_length = 0;
}

void EagleDbParser_Delete()
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

int EagleDbParser_LoadString(const char *str)
{
    return yy_scan_string(str);
}

int EagleDbParser_Parse(void)
{
    return yyparse();
}
