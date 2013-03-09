#include <string.h>
#include "EagleDataType.h"

EagleDataType EagleDataType_nameToType(char *name)
{
    if(!strcasecmp(name, "int") || !strcasecmp(name, "integer")) {
        return EagleDataTypeInteger;
    }
    if(!strcasecmp(name, "varchar") || !strcasecmp(name, "text")) {
        return EagleDataTypeVarchar;
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
            
        case EagleDataTypeVarchar:
            return strdup("VARCHAR");
            
        case EagleDataTypeFloat:
            return strdup("FLOAT");
            
    }
}
