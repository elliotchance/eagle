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
    
    /**
     Error stack.
     */
    EAGLE_ATTR_MANAGED char **yyerrors;
    
    /**
     Total number of errors.
     */
    EAGLE_ATTR_NA int yyerrors_length;
    
    /**
     This is the pointer to the final AST returned by the parser.
     */
    EAGLE_ATTR_MANAGED void *yyparse_ast;
    
    /**
     Object stack. This is used when something goes wrong it can free all the objects that were allocated during the
     parsing process.
     */
    EAGLE_ATTR_MANAGED void **yyobj;
    
    /**
     The number of objects in the stack.
     */
    EAGLE_ATTR_NA int yyobj_length;
    
    /**
     When returning an array of something you can use this mechanism.
     */
    EAGLE_ATTR_MANAGED void **yylist;
    
    /**
     The number of items in the yylist instance.
     */
    EAGLE_ATTR_NA int yylist_length;
    
    /**
     A return stack. Maximum depth is 256.
     */
    EAGLE_ATTR_MANAGED void **yyreturn;
    
    /**
     The numbero of items in the return stack.
     */
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
 Load a string into the parser. This does not start the parser.
 @return 0 on success, any other number is a failure.
 @see EagleDbParser_Parse()
 */
int EagleDbParser_LoadString(const char *str);

/**
 This can be used to get the most recent yytext token. This is not a data duplication of the token so you must copy it
 out if you intended to keep it.
 */
char* EagleDbParser_LastToken(void);

/**
 Parse the loaded content now.
 */
int EagleDbParser_Parse(void);

/**
 This function must be provided to give flex and bison somewhere to send the errors. They will be put onto a proper
 stack once received.
 */
int yyerror(char *s);

/**
 Prepare the default parser. This must be invoked before the SQL is parsed.
 */
void EagleDbParser_Init(void);

/**
 Clean up any internal resources associated with the most recent parse.
 */
void EagleDbParser_Delete(void);

/**
 Get the last error from the parser.
 @return The last error message.
 */
char* EagleDbParser_LastError(void);

char* EagleDbParser_AddError(void *ptr);

void* EagleDbParser_AddObject(void *ptr);

void* yylist_push(void *ptr);

void* yylist_new(void);

void* EagleDbParser_AddReturn(void *ptr);

void* EagleDbParser_PopReturn(void);

void* EagleDbParser_CurrentReturn(void);

#endif
