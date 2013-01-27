#ifndef eagle_EagleDbParser_h
#define eagle_EagleDbParser_h

#include "EagleData.h"
#include "EagleDbSqlStatementType.h"

#define MAX_YYOBJ 256
#define MAX_YYERRORS 100
#define MAX_YYRETURN 256
#define MAX_YYLIST 256

typedef struct {
    
    /**
     The type of statement.
     */
    EAGLE_ATTR_NA EagleDbSqlStatementType yystatementtype;
    
    EAGLE_ATTR_MANAGED char **yyerrors;
    
    EAGLE_ATTR_NA int yyerrors_length;
    
    /**
     This is the pointer to the final AST returned by the parser.
     */
    EAGLE_ATTR_MANAGED void *yyparse_ast;
    
    EAGLE_ATTR_MANAGED void **yyobj;
    
    EAGLE_ATTR_NA int yyobj_length;
    
    /**
     When returning an array of something you can use this mechanism.
     */
    EAGLE_ATTR_MANAGED void **yylist;
    
    EAGLE_ATTR_NA int yylist_length;
    
    /**
     A return stack. Maximum depth is 256.
     */
    EAGLE_ATTR_MANAGED void **yyreturn;
    
    EAGLE_ATTR_NA int yyreturn_length;
    
    /**
     Used by "data_type"
     */
    EAGLE_ATTR_NA EagleDataType yy_data_type;
    
} EagleDbParser;

/**
 Create a new parser.
 
 @note Flex and bison are not thread safe so this is actually just an internal function that is called when its needed.
       You should use EagleDbParser_Get()
 @return A new parser instance.
 */
EagleDbParser* EagleDbParser_New(void);

/**
 Get the default parser.
 @return The default parser.
 */
EagleDbParser* EagleDbParser_Get(void);

/**
 Parse a string.
 @return 0 on success, any other number is a failure.
 */
int EagleDbParser_ParseString(const char *str);

/**
 This can be used to get the most recent yytext token. This is not a data duplication of the token so you must copy it
 out if you intended to keep it.
 */
extern char *yytext_last;

/**
 Internal prototype provided by flex and bison.
 */
int yyparse(void);

/**
 This function must be provided to give flex and bison somewhere to send the errors. They will be put onto a proper
 stack once received.
 */
int yyerror(char *s);

int yylex(void);

char* yyerrors_last(void);

/**
 Prepare the default parser. This must be invoked before the SQL is parsed.
 */
void EagleDbParser_Init(void);

/**
 Clean up any internal resources associated with the most recent parse.
 */
void EagleDbParser_Delete(void);

char* yyerrors_push(void *ptr);

void* yyobj_push(void *ptr);

void* yylist_push(void *ptr);

void* yylist_new(void);

void* yyreturn_push(void *ptr);

void* yyreturn_pop(void);

void* yyreturn_current(void);

#endif
