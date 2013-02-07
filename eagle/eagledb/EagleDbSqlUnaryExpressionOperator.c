#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "EagleDbSqlUnaryExpressionOperator.h"

void EagleDbSqlUnaryExpressionOperator_toString(EagleDbSqlUnaryExpressionOperator op, char **before, char **after)
{
    switch(op) {
        
        case EagleDbSqlUnaryExpressionOperatorNegate:
            *before = strdup("-");
            *after = strdup("");
            break;
            
        case EagleDbSqlUnaryExpressionOperatorNot:
            *before = strdup("NOT ");
            *after = strdup("");
            break;
            
        case EagleDbSqlUnaryExpressionOperatorGrouping:
            *before = strdup("(");
            *after = strdup(")");
            break;
            
    }
}
