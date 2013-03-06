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
     Header.
     @see EagleDbSqlExpressionHeader
     */
    EagleDbSqlExpressionHeader;
    
    /**
     Value type.
     @see EagleDbSqlValueType
     */
    EAGLE_ATTR_NA EagleDbSqlValueType type;
    
    /**
     Contains the actual value, depending on the type.
     */
    union {
        
        /**
         Integer.
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
 * Create a new EagleDbSqlValue with a string literal.
 * @param [in] str String value.
 * @param [in] process If this is EagleTrue then the string will be processed, ie remove the surrounding quotes and
 * unescape inner quotes.
 * @return A new EagleDbSqlValue instance.
 */
EagleDbSqlValue* EagleDbSqlValue_NewWithString(const char *str, EagleBoolean process);

/**
 * Create a new EagleDbSqlValue with an integer.
 * @param [in] value Integer value.
 * @return A new EagleDbSqlValue instance.
 */
EagleDbSqlValue* EagleDbSqlValue_NewWithInteger(int value);

/**
 * Create a new EagleDbSqlValue with an asterisk token.
 * @return A new EagleDbSqlValue instance.
 */
EagleDbSqlValue* EagleDbSqlValue_NewWithAsterisk(void);

/**
 * Create a new EagleDbSqlValue with an identifier (a column name).
 * @param [in] name The name of the column.
 * @return A new EagleDbSqlValue instance.
 */
EagleDbSqlValue* EagleDbSqlValue_NewWithIdentifier(char *name);

/**
 * Free a value.
 * @param [in] value The instance.
 */
void EagleDbSqlValue_Delete(EagleDbSqlValue *value);

/**
 * Render a value to a string.
 * @param [in] value The instance.
 * @return A new string representation of the value.
 */
char* EagleDbSqlValue_toString(EagleDbSqlValue *value);

#endif
