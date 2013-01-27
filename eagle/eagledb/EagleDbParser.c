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
    
    parser->errors = EagleLinkedList_New();
    parser->returns = EagleLinkedList_New();
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

EagleBoolean EagleDbParser_HasError(void)
{
    return !EagleLinkedList_isEmpty(EagleDbParser_Get()->errors);
}

char* EagleDbParser_AddError(void *ptr)
{
    EagleDbParser *p = EagleDbParser_Default;
    EagleLinkedList_add(p->errors, EagleLinkedListItem_New(ptr, EagleTrue, NULL));
    return ptr;
}

char* EagleDbParser_LastError()
{
    EagleDbParser *p = EagleDbParser_Default;
    return (char*) EagleLinkedList_end(p->errors)->obj;
}

void* EagleDbParser_AddReturn(void *ptr)
{
    EagleDbParser *p = EagleDbParser_Default;
    EagleLinkedList_add(p->returns, EagleLinkedListItem_New(ptr, EagleFalse, NULL));
    return ptr;
}

void* EagleDbParser_PopReturn()
{
    EagleDbParser *p = EagleDbParser_Default;
    EagleLinkedListItem *item = EagleLinkedList_pop(p->returns);
    void *obj = item->obj;
    EagleLinkedListItem_Delete(item);
    return obj;
}

void* EagleDbParser_CurrentReturn()
{
    EagleDbParser *p = EagleDbParser_Default;
    return EagleLinkedList_end(p->returns)->obj;
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
}

void EagleDbParser_Delete()
{
    EagleDbParser *p = EagleDbParser_Default;
    
    EagleLinkedList_Delete(p->returns);
    EagleLinkedList_DeleteWithItems(p->errors);
    
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
