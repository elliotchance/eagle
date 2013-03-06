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

EagleDbSqlValue* EagleDbSqlValue_NewWithString(const char *str, EagleBoolean process)
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
        unsigned long len, i;
        char *pstr;
        
        len = strlen(str);
        pstr = (char*) EagleMemory_Allocate("EagleDbSqlValue_NewWithString.2", len + 1);
        if(NULL == pstr) {
            EagleMemory_Free(v);
            return NULL;
        }
        
        for(i = 0; i < len - 1; ++i) {
            pstr[i] = str[i + 1];
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
