#include <stdlib.h>
#include "EagleDbSqlValue.h"

/**
 Create a new Value with an integer.
 
 @param value Integer value.
 */
EagleDbSqlValue* EagleDbSqlValue_NewWithInteger(int value)
{
    EagleDbSqlValue *v = (EagleDbSqlValue*) malloc(sizeof(EagleDbSqlValue));
    
    v->expressionType = EagleDbSqlExpressionTypeValue;
    v->type = EagleDbSqlValueTypeInteger;
    v->value.intValue = value;
    
    return v;
}

void EagleDbSqlValue_Delete(EagleDbSqlValue *value)
{
    free((void*) value);
}
