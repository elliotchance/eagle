#ifndef eagle_EagleDbParser_h
#define eagle_EagleDbParser_h

#include "EagleData.h"
#include "EagleDbSqlStatementType.h"
#include "EagleLinkedList.h"

typedef struct {
    
    /**
     The type of statement.
     */
    EAGLE_ATTR_NA EagleDbSqlStatementType yystatementtype;
    
    /**
     Error stack.
     */
    EAGLE_ATTR_MANAGED EagleLinkedList *errors;
    
    /**
     This is the pointer to the final AST returned by the parser.
     */
    EAGLE_ATTR_MANAGED void *yyparse_ast;
    
    /**
     A return stack.
     */
    EAGLE_ATTR_MANAGED EagleLinkedList *returns;
    
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
void EagleDbParser_Finish(void);

/**
 Get the last error from the parser.
 @return The last error message.
 */
char* EagleDbParser_LastError(void);

/**
 Push the error onto the stack.
 */
char* EagleDbParser_AddError(void *ptr);

/**
 Push the return value onto the stack.
 */
void* EagleDbParser_AddReturn(void *ptr, void (*free)(void*));

/**
 Return the last yyreturn and decrement back the stack.
 */
void* EagleDbParser_PopReturn(void);

/**
 Return the most recent yyreturn.
 */
void* EagleDbParser_CurrentReturn(void);

/**
 Did the parser encounter any errors?
 */
EagleBoolean EagleDbParser_HasError(void);

#endif
