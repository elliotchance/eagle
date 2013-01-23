#ifndef eagle_EaglePlanError_h
#define eagle_EaglePlanError_h

/**
 Represents the types of errors (or successes) that can occur inside a plan.
 */
typedef enum {
    
    EaglePlanErrorNone = 0,       /**< Everything is fine. */
    EaglePlanErrorCompile = 1,    /**< If an expression cannot be compiled (i.e. syntax error) */
    EaglePlanErrorIdentifier = 2, /**< Unknown column / identifier. */
    EaglePlanErrorNoSuchTable = 4 /**< The table name does not exist in the schema. */
    
} EaglePlanError;

#endif
