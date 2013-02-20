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
     Pointer to allow the reentrant parser to work.
     */
    EAGLE_ATTR_MANAGED void *yyparse;
    
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
     A symbol provided by flex that contains the last token read.
     */
    EAGLE_ATTR_MANAGED char *yytext_last;
    
} EagleDbParser;

/**
 * Create a new parser.
 * @return A new parser instance.
 */
EagleDbParser* EagleDbParser_New(void);

EagleDbParser* EagleDbParser_ParseWithString(const char *str);

/**
 * This can be used to get the most recent yytext token. This is not a data duplication of the token so you must copy it
 * out if you intended to keep it.
 * @return The last token.
 */
char* EagleDbParser_lastToken(EagleDbParser *p);

/**
 * This function must be provided to give flex and bison somewhere to send the errors. They will be put onto a proper
 * stack once received.
 * @param [in] s The message.
 * @return Ignored.
 */
int yyerror(EagleDbParser *parser, void *scanner, char *s);

/**
 * Delete the parser instance.
 */
void EagleDbParser_Delete(EagleDbParser *p);

/**
 * Get the last error from the parser.
 * @return The last error message.
 */
char* EagleDbParser_lastError(EagleDbParser *p);

/**
 * Push the error onto the stack.
 * @param [in] message The error message.
 */
void EagleDbParser_addError(EagleDbParser *p, char *message);

/**
 * Did the parser encounter any errors?
 * @return EagleTrue if the parser encountered an error.
 */
EagleBoolean EagleDbParser_hasError(EagleDbParser *p);

#endif
