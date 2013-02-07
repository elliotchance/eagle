#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Eagle.h"
#include "EagleDbParser.h"
#include "EagleDbSqlStatementType.h"
#include "EagleMemory.h"
#include "EagleData.h"
#include "EagleDbSqlExpression.h"

/**
 Since the parser is synchronized and basically static we reuse the same internal parser instance.
 */
static EagleDbParser *EagleDbParser_Default = NULL;

/* internal prototypes supplied by flex and bison */
extern int yylex_destroy(void);
extern int yy_scan_string(const char *str);
extern int yyparse();

/**
 A symbol provided by flex that contains the last token read.
 */
extern char *yytext_last;

EagleDbParser* EagleDbParser_New(void)
{
    EagleDbParser *parser = (EagleDbParser*) EagleMemory_Allocate("EagleDbParser_New.1", sizeof(EagleDbParser));
    if(NULL == parser) {
        return NULL;
    }
    
    parser->errors = EagleLinkedList_New();
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
    EagleDbParser *p = EagleDbParser_Get();
    return !EagleLinkedList_isEmpty(p->errors);
}

void* EagleDbParser_AddError(void *ptr)
{
    EagleDbParser *p = EagleDbParser_Default;
    EagleLinkedList_add(p->errors, EagleLinkedListItem_New(ptr, EagleTrue, NULL));
    return ptr;
}

char* EagleDbParser_LastError()
{
    EagleDbParser *p = EagleDbParser_Default;
    if(EagleLinkedList_isEmpty(p->errors)) {
        return NULL;
    }
    return (char*) EagleLinkedList_end(p->errors)->obj;
}

int yyerror(char *s)
{
    EagleDbParser_AddError(strdup(s));
    return 0;
}

void EagleDbParser_Init()
{
    EagleDbParser_Default = EagleDbParser_New();
}

void EagleDbParser_Finish()
{
    EagleDbParser *p = EagleDbParser_Default;
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

void EagleDbParser_SetStatementType(EagleDbSqlStatementType type)
{
    EagleDbParser *p = EagleDbParser_Default;
    p->yystatementtype = type;
}
