#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "EagleDbSqlExpressionOperator.h"

char* EagleDbSqlExpressionOperator_toString(EagleDbSqlExpressionOperator op)
{
    switch(op) {
            
        case EagleDbSqlExpressionOperatorPlus:
            return strdup("+");
            
        case EagleDbSqlExpressionOperatorEquals:
            return strdup("=");
            
        case EagleDbSqlExpressionOperatorModulus:
            return strdup("%");
            
        case EagleDbSqlExpressionOperatorMultiply:
            return strdup("*");
            
    }
}
