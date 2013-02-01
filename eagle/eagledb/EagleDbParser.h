#ifndef eagle_EagleDbParser_h
#define eagle_EagleDbParser_h

#include "EagleData.h"
#include "EagleDbSqlStatementType.h"
#include "EagleLinkedList.h"

/**
 A instance to handle the parser.
 */
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
    
    /**
     A managed stack of allocated objects to be freed when an error occurs.
     */
    EAGLE_ATTR_MANAGED EagleLinkedList *objects;
    
} EagleDbParser;

/**
 * Create a new parser.
 * @note Flex and bison are not thread safe so this is actually just an internal function that is called when its
 * needed. You should use EagleDbParser_Get()
 * @return A new parser instance.
 */
EagleDbParser* EagleDbParser_New(void);

/**
 * Get the default parser.
 * @return The default parser.
 */
EagleDbParser* EagleDbParser_Get(void);

/**
 * Load a string into the parser. This does not start the parser.
 * @param [in] str The string to parse.
 * @return 0 on success, any other number is a failure.
 * @see EagleDbParser_Parse()
 */
int EagleDbParser_LoadString(const char *str);

/**
 * This can be used to get the most recent yytext token. This is not a data duplication of the token so you must copy it
 * out if you intended to keep it.
 * @return The last token.
 */
char* EagleDbParser_LastToken(void);

/**
 * Parse the loaded content now.
 * @return 0 on success.
 */
int EagleDbParser_Parse(void);

/**
 * This function must be provided to give flex and bison somewhere to send the errors. They will be put onto a proper
 * stack once received.
 * @param [in] s The message.
 * @return Ignored.
 */
int yyerror(char *s);

/**
 * Prepare the default parser. This must be invoked before the SQL is parsed.
 */
void EagleDbParser_Init(void);

/**
 * Clean up any internal resources associated with the most recent parse.
 */
void EagleDbParser_Finish(void);

/**
 * Get the last error from the parser.
 * @return The last error message.
 */
char* EagleDbParser_LastError(void);

/**
 * Push the error onto the stack.
 * @param [in] ptr The object.
 * @return \p ptr
 */
void* EagleDbParser_AddError(void *ptr);

/**
 * Push the return value onto the stack.
 * @param [in] ptr The object.
 * @return \p ptr
 */
void* EagleDbParser_AddReturn(void *ptr);

/**
 * Return the last yyreturn and decrement back the stack.
 * @return the last yyreturn
 */
void* EagleDbParser_PopReturn(void);

/**
 * Return the most recent yyreturn.
 * @return the most recent yyreturn.
 */
void* EagleDbParser_CurrentReturn(void);

/**
 * Did the parser encounter any errors?
 * @return EagleTrue if the parser encountered an error.
 */
EagleBoolean EagleDbParser_HasError(void);

/**
 * Add an object to be freed if an error occurs.
 * @param ptr The object.
 * @param free The function to use when freeing the object. You may use NULL for EagleMemory_Free()
 * @return 
 */
void* EagleDbParser_AddObject(void *ptr, void (*free)(void*));

#endif
