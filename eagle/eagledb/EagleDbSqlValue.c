#include <stdlib.h>
#include "EagleDbSqlValue.h"

EagleDbSqlValue* EagleDbSqlValue_NewWithInteger(int value)
{
    EagleDbSqlValue *v = (EagleDbSqlValue*) malloc(sizeof(EagleDbSqlValue));
    v->type = EagleDbSqlValueTypeInteger;
    v->value.intValue = value;
    return v;
}
