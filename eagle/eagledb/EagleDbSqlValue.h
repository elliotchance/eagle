#ifndef eagle_EagleDbSqlValue_h
#define eagle_EagleDbSqlValue_h

#include "EagleDbSqlExpression.h"
#include "EagleDbSqlValueType.h"
#include "Eagle.h"

/**
 Encapsulates a Value of any type.
 */
typedef struct {
    
    /**
     @see EagleDbSqlExpressionHeader
     */
    EagleDbSqlExpressionHeader;
    
    /**
     @see EagleDbSqlValueType
     */
    EAGLE_ATTR_NA EagleDbSqlValueType type;
    
    union {
        
        /**
         @see EagleDbSqlValueTypeInteger
         */
        EAGLE_ATTR_NA int intValue;
        
        /**
         When using identifier token. This is the name of the identifier.
         */
        EAGLE_ATTR_MANAGED char *identifier;
        
    } value;
    
} EagleDbSqlValue;

/**
 Create a new Value with an integer.
 
 @param [in] value Integer value.
 */
EagleDbSqlValue* EagleDbSqlValue_NewWithInteger(int value);

EagleDbSqlValue* EagleDbSqlValue_NewWithAsterisk(void);

EagleDbSqlValue* EagleDbSqlValue_NewWithIdentifier(char *name);

void EagleDbSqlValue_Delete(EagleDbSqlValue *value);

char* EagleDbSqlValue_toString(EagleDbSqlValue *value);

#endif
