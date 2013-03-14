#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "EagleDbSqlValue.h"
#include "EagleMemory.h"

EagleDbSqlValue* EagleDbSqlValue_NewWithInteger(EagleDataTypeIntegerType value)
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

EagleDbSqlValue* EagleDbSqlValue_NewWithFloat(EagleDataTypeFloatType value)
{
    EagleDbSqlValue *v = (EagleDbSqlValue*) EagleMemory_Allocate("EagleDbSqlValue_NewWithFloat.1", sizeof(EagleDbSqlValue));
    if(NULL == v) {
        return NULL;
    }
    
    v->expressionType = EagleDbSqlExpressionTypeValue;
    v->type = EagleDbSqlValueTypeFloat;
    v->value.floatValue = value;
    
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

EagleDbSqlValue* EagleDbSqlValue_NewWithString(char *str, EagleBoolean process)
{
    EagleDbSqlValue *v = (EagleDbSqlValue*) EagleMemory_Allocate("EagleDbSqlValue_NewWithString.1", sizeof(EagleDbSqlValue));
    if(NULL == v) {
        return NULL;
    }
    
    v->expressionType = EagleDbSqlExpressionTypeValue;
    v->type = EagleDbSqlValueTypeString;
    
    if(EagleFalse == process) {
        v->value.identifier = (NULL == str ? NULL : strdup(str));
    }
    else {
        /* process the string */
        unsigned long len, i, j;
        char *pstr;
        
        len = strlen(str);
        pstr = (char*) EagleMemory_Allocate("EagleDbSqlValue_NewWithString.2", len + 1);
        if(NULL == pstr) {
            EagleMemory_Free(v);
            return NULL;
        }
        
        for(i = 1, j = 0; i < len - 1; ++i, ++j) {
            if((str[i] == '\\' && str[i + 1] == '\'') || (str[i] == '\'' && str[i + 1] == '\'')) {
                pstr[j] = '\'';
                ++i;
            }
            else {
                pstr[j] = str[i];
            }
        }
        pstr[len - 2] = '\0';
        
        v->value.identifier = (char*) pstr;
    }
    
    return v;
}

void EagleDbSqlValue_Delete(EagleDbSqlValue *value)
{
    if(NULL == value) {
        return;
    }
    
    switch(value->type) {
            
        case EagleDbSqlValueTypeAsterisk:
        case EagleDbSqlValueTypeInteger:
        case EagleDbSqlValueTypeFloat:
            break;
            
        case EagleDbSqlValueTypeIdentifier:
        case EagleDbSqlValueTypeString:
            EagleMemory_Free(value->value.identifier);
            break;
            
    }
    
    EagleMemory_Free(value);
}

char* EagleDbSqlValue_toString(EagleDbSqlValue *value)
{
    if(NULL == value) {
        return NULL;
    }
    
    switch(value->type) {
            
        case EagleDbSqlValueTypeInteger:
        {
            char buf[32];
            sprintf(buf, "%d", value->value.intValue);
            return strdup(buf);
        }
            
        case EagleDbSqlValueTypeFloat:
        {
            char buf[32];
            sprintf(buf, "%g", value->value.floatValue);
            return strdup(buf);
        }
            
        case EagleDbSqlValueTypeAsterisk:
            return strdup("*");
            
        case EagleDbSqlValueTypeIdentifier:
            return strdup(value->value.identifier);
            
        case EagleDbSqlValueTypeString:
        {
            char *buf = (char*) EagleMemory_Allocate("EagleDbSqlValue_toString.1", strlen(value->value.identifier) + 3);
            if(NULL == buf) {
                return NULL;
            }
            
            sprintf(buf, "'%s'", value->value.identifier);
            return buf;
        }

    }
}

EagleDataTypeIntegerType EagleDbSqlValue_getInteger(EagleDbSqlValue *value, EagleBoolean *success)
{
    switch(value->type) {
            
        case EagleDbSqlValueTypeAsterisk:
        case EagleDbSqlValueTypeIdentifier:
        case EagleDbSqlValueTypeString:
            *success = EagleFalse;
            return 0;
            
        case EagleDbSqlValueTypeFloat:
            *success = EagleTrue;
            return (EagleDataTypeIntegerType) value->value.floatValue;
            
        case EagleDbSqlValueTypeInteger:
            *success = EagleTrue;
            return value->value.intValue;
            
    }
}

EagleDataTypeFloatType EagleDbSqlValue_getFloat(EagleDbSqlValue *value, EagleBoolean *success)
{
    switch(value->type) {
            
        case EagleDbSqlValueTypeAsterisk:
        case EagleDbSqlValueTypeIdentifier:
        case EagleDbSqlValueTypeString:
            *success = EagleFalse;
            return 0.0;
            
        case EagleDbSqlValueTypeFloat:
            *success = EagleTrue;
            return value->value.floatValue;
            
        case EagleDbSqlValueTypeInteger:
            *success = EagleTrue;
            return (EagleDataTypeFloatType) value->value.intValue;
            
    }
}

EagleDataTypeVarcharType EagleDbSqlValue_getVarchar(EagleDbSqlValue *value, EagleBoolean *success)
{
    switch(value->type) {
            
        case EagleDbSqlValueTypeAsterisk:
        case EagleDbSqlValueTypeIdentifier:
        case EagleDbSqlValueTypeFloat:
        case EagleDbSqlValueTypeInteger:
            *success = EagleFalse;
            return NULL;
            
        case EagleDbSqlValueTypeString:
            *success = EagleTrue;
            return value->value.identifier;
            
    }
}
