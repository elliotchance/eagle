#include <string.h>
#include "EagleDbSqlValueType.h"

char* EagleDbSqlValueType_toString(EagleDbSqlValueType type)
{
    switch(type) {
            
        case EagleDbSqlValueTypeAsterisk:
            return strdup("Asterisk");
            
        case EagleDbSqlValueTypeFloat:
            return strdup("FLOAT");
            
        case EagleDbSqlValueTypeIdentifier:
            return strdup("Identifier");
            
        case EagleDbSqlValueTypeInteger:
            return strdup("INTEGER");
            
        case EagleDbSqlValueTypeString:
            return strdup("VARCHAR");
            
    }
}
