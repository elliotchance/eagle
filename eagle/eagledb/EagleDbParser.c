#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Eagle.h"
#include "EagleDbParser.h"
#include "EagleDbSqlStatementType.h"
#include "EagleMemory.h"
#include "EagleData.h"
#include "EagleDbSqlExpression.h"
#include "EagleUtils.h"

/**
 Trivial internal type.
 */
typedef void* yyscan_t;

/* internal prototypes supplied by flex and bison */
extern int yylex_destroy(yyscan_t);
extern int yy_scan_string(const char *str, yyscan_t scanner);
extern int yyparse(EagleDbParser*, yyscan_t);

int yylex_init(yyscan_t *ptr_yy_globals);
int yylex_init_extra(EagleDbParser *user_defined, yyscan_t *ptr_yy_globals);
int yylex(yyscan_t yyscanner);
int yylex_destroy(yyscan_t yyscanner);

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

EagleDbParser* EagleDbParser_ParseWithString(const char *sql)
{
    EagleDbParser *parser = EagleDbParser_New();
    
    yylex_init_extra(parser, &parser->yyparse);
    if(NULL != parser) {
        yy_scan_string(sql, parser->yyparse);
        yyparse(parser, parser->yyparse);
    }
    
    return parser;
}

char* EagleDbParser_lastToken(EagleDbParser *p)
{
    return p->yytext_last;
}

EagleBoolean EagleDbParser_hasError(EagleDbParser *p)
{
    return !EagleLinkedList_isEmpty(p->errors);
}

void EagleDbParser_addError(EagleDbParser *p, char *message)
{
    EagleLinkedList_add(p->errors, EagleLinkedListItem_New(strdup(message), EagleTrue, NULL));
}

char* EagleDbParser_lastError(EagleDbParser *p)
{
    if(NULL == p) {
        return NULL;
    }
    if(EagleLinkedList_isEmpty(p->errors)) {
        return NULL;
    }
    return (char*) EagleLinkedList_end(p->errors)->obj;
}

int yyerror(EagleDbParser *parser, void *scanner, char *s)
{
    EagleDbParser_addError(parser, s);
    return 0;
}

void EagleDbParser_Delete(EagleDbParser *p)
{
    if(NULL == p) {
        return;
    }
    
    EagleLinkedList_DeleteWithItems(p->errors);
    if(NULL != p->yyparse) {
        yylex_destroy(p->yyparse);
    }
    EagleMemory_Free(p);
}

EagleBoolean EagleDbParser_IsKeyword(const char *word)
{
    unsigned long i;
    static const char *words[] = {
        "CREATE",
        "FROM",
        "INT",
        "INTEGER",
        "SELECT",
        "TABLE",
        "TEXT",
        "WHERE",
        "VALUES",
        "INSERT",
        "INTO",
        "AND",
        "OR",
        "NOT"
    };
    
    for(i = 0; i < sizeof(words) / sizeof(const char*); ++i) {
        if(EagleTrue == EagleUtils_CompareWithoutCase(word, words[i])) {
            return EagleTrue;
        }
    }
    
    return EagleFalse;
}
