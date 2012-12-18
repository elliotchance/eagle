#include <stdlib.h>
#include <string.h>
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

EagleDbSqlValue* EagleDbSqlValue_NewWithAsterisk(void)
{
    EagleDbSqlValue *v = (EagleDbSqlValue*) malloc(sizeof(EagleDbSqlValue));
    
    v->expressionType = EagleDbSqlExpressionTypeValue;
    v->type = EagleDbSqlValueTypeAsterisk;
    
    return v;
}

EagleDbSqlValue* EagleDbSqlValue_NewWithIdentifier(char *identifier)
{
    EagleDbSqlValue *v = (EagleDbSqlValue*) malloc(sizeof(EagleDbSqlValue));
    
    v->expressionType = EagleDbSqlExpressionTypeValue;
    v->type = EagleDbSqlValueTypeIdentifier;
    v->value.identifier = strdup(identifier);
    
    return v;
}

void EagleDbSqlValue_Delete(EagleDbSqlValue *value)
{
    if(EagleDbSqlValueTypeIdentifier == value->type) {
        free(value->value.identifier);
    }
    free((void*) value);
}
