#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "EagleDbSqlValue.h"
#include "EagleMemory.h"

EagleDbSqlValue* EagleDbSqlValue_NewWithInteger(int value)
{
    EagleDbSqlValue *v = (EagleDbSqlValue*) EagleMemory_Allocate("EagleDbSqlValue_NewWithInteger.1", sizeof(EagleDbSqlValue));
    if(NULL == v) {
        return NULL;
    }
    
    v->expressionType = EagleDbSqlExpressionTypeValue;
    v->type = EagleDbSqlValueTypeInteger;
    v->value.intValue = value;
    
    return v;
}

EagleDbSqlValue* EagleDbSqlValue_NewWithAsterisk(void)
{
    EagleDbSqlValue *v = (EagleDbSqlValue*) EagleMemory_Allocate("EagleDbSqlValue_NewWithAsterisk.1", sizeof(EagleDbSqlValue));
    if(NULL == v) {
        return NULL;
    }
    
    v->expressionType = EagleDbSqlExpressionTypeValue;
    v->type = EagleDbSqlValueTypeAsterisk;
    
    return v;
}

EagleDbSqlValue* EagleDbSqlValue_NewWithIdentifier(char *identifier)
{
    EagleDbSqlValue *v = (EagleDbSqlValue*) EagleMemory_Allocate("EagleDbSqlValue_NewWithIdentifier.1", sizeof(EagleDbSqlValue));
    if(NULL == v) {
        return NULL;
    }
    
    v->expressionType = EagleDbSqlExpressionTypeValue;
    v->type = EagleDbSqlValueTypeIdentifier;
    v->value.identifier = (NULL == identifier ? NULL : strdup(identifier));
    
    return v;
}

void EagleDbSqlValue_Delete(EagleDbSqlValue *value)
{
    if(NULL == value) {
        return;
    }
    
    if(EagleDbSqlValueTypeIdentifier == value->type) {
        EagleMemory_Free(value->value.identifier);
    }
    EagleMemory_Free(value);
}

char* EagleDbSqlValue_toString(EagleDbSqlValue *value)
{
    switch(value->type) {
        case EagleDbSqlValueTypeInteger:
        {
            char buf[32];
            sprintf(buf, "%d", value->value.intValue);
            return strdup(buf);
        }
            
        case EagleDbSqlValueTypeAsterisk:
            return strdup("*");
            
        case EagleDbSqlValueTypeIdentifier:
            return strdup(value->value.identifier);

    }
}
