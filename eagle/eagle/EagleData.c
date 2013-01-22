#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "EagleData.h"
#include "EagleUtils.h"
#include "EagleMemory.h"

int* EagleData_Int(int value)
{
    int *r = (int*) EagleMemory_Allocate("EagleData_Int.1", sizeof(int));
    if(NULL == r) {
        return NULL;
    }
    *r = value;
    return r;
}

EagleDataType EagleDataType_nameToType(char *name)
{
    if(!strcasecmp(name, "int") || !strcasecmp(name, "integer")) {
        return EagleDataTypeInteger;
    }
    if(!strcasecmp(name, "text")) {
        return EagleDataTypeText;
    }
    
    return EagleDataTypeUnknown;
}

char* EagleDataType_typeToName(EagleDataType type)
{
    switch(type) {
            
        case EagleDataTypeUnknown:
            return strdup("UNKNOWN");
            
        case EagleDataTypeInteger:
            return strdup("INTEGER");
            
        case EagleDataTypeText:
            return strdup("TEXT");
            
    }
}
