#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "EagleData.h"
#include "EagleUtils.h"
#include "EagleMemory.h"

/**
 Allocate an integer and return the pointer.
 
 @param [in] value Value to set into newly allocated space.
 */
int* EagleData_Int(int value)
{
    int *r = (int*) EagleMemory_Allocate("EagleData_Int.1", sizeof(int));
    if(NULL == r) {
        return NULL;
    }
    *r = value;
    return r;
}

/**
 Convert a type name to its enum value. This function is not case-sensitive.
 @param [in] name The name of the type.
 @return EagleDataTypeUnknown if the type is unknown. Otherwise the correct enum value for the type.
 @see EagleDataType
 */
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

/**
 Convert a type to a string.
 @param [in] type The data type.
 @return A new string (you must free it)
 @see EagleDataType
 */
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
