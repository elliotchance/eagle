#ifndef eagle_EagleDbSqlValue_h
#define eagle_EagleDbSqlValue_h

#include "EagleDbSqlExpression.h"

/**
 A value can contain several types.
 
 The first member variable of EagleDbSqlValue will indicate the real type of the Value.
 */
typedef enum {
    
    /**
     Use \p intValue
     */
    EagleDbSqlValueTypeInteger = 1,
    
    /**
     Asterisk expression for selecting multiple columns.
     */
    EagleDbSqlValueTypeAsterisk = 2
    
} EagleDbSqlValueType;

/**
 Encapsulates a Value of any type.
 */
typedef struct {
    
    EagleDbSqlExpressionHeader;
    
    /**
     @see EagleDbSqlValueType
     */
    EagleDbSqlValueType type;
    
    union {
        
        /**
         @see EagleDbSqlValueTypeInteger
         */
        int intValue;
        
    } value;
    
} EagleDbSqlValue;

EagleDbSqlValue* EagleDbSqlValue_NewWithInteger(int value);
EagleDbSqlValue* EagleDbSqlValue_NewWithAsterisk(void);
void EagleDbSqlValue_Delete(EagleDbSqlValue *value);

#endif
