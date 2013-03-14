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
        EAGLE_ATTR_NA EagleDataTypeIntegerType intValue;
        
        /**
         When using identifier token. This is the name of the identifier.
         */
        EAGLE_ATTR_MANAGED char *identifier;
        
        /**
         Floating point number.
         @see EagleDbSqlValueTypeFloat
         */
        EAGLE_ATTR_NA EagleDataTypeFloatType floatValue;
        
    } value;
    
} EagleDbSqlValue;

/**
 * Create a new EagleDbSqlValue with a string literal.
 * @param [in] str String value.
 * @param [in] process If this is EagleTrue then the string will be processed, ie remove the surrounding quotes and
 * unescape inner quotes.
 * @return A new EagleDbSqlValue instance.
 */
EagleDbSqlValue* EagleDbSqlValue_NewWithString(char *str, EagleBoolean process);

/**
 * Create a new EagleDbSqlValue with an integer.
 * @param [in] value Integer value.
 * @return A new EagleDbSqlValue instance.
 */
EagleDbSqlValue* EagleDbSqlValue_NewWithInteger(EagleDataTypeIntegerType value);

/**
 * Create a new EagleDbSqlValue with an floating point number.
 * @param [in] value Floating point value.
 * @return A new EagleDbSqlValue instance.
 */
EagleDbSqlValue* EagleDbSqlValue_NewWithFloat(EagleDataTypeFloatType value);

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

/**
 Get the integer representation of a value.
 @param [in] value The value.
 @param [out] success Set ot EagleTrue if the returned value is correct.
 @return The integer value of whatever the original type of the \p value was. If the value could now be cast then
 \p success will be set to EagleFalse and 0 will be returned.
 */
EagleDataTypeIntegerType EagleDbSqlValue_getInteger(EagleDbSqlValue *value, EagleBoolean *success);

/**
 Get the float representation of a value.
 @param [in] value The value.
 @param [out] success Set ot EagleTrue if the returned value is correct.
 @return The float value of whatever the original type of the \p value was. If the value could now be cast then
 \p success will be set to EagleFalse and 0.0 will be returned.
 */
EagleDataTypeFloatType EagleDbSqlValue_getFloat(EagleDbSqlValue *value, EagleBoolean *success);

/**
 Get the varchar representation of a value.
 @param [in] value The value.
 @param [out] success Set ot EagleTrue if the returned value is correct.
 @return The varchar value of whatever the original type of the \p value was. If the value could now be cast then
 \p success will be set to EagleFalse and NULL will be returned.
 */
EagleDataTypeVarcharType EagleDbSqlValue_getVarchar(EagleDbSqlValue *value, EagleBoolean *success);

#endif
