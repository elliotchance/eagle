#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "EagleDbSqlUnaryExpressionOperator.h"

char* EagleDbSqlUnaryExpressionOperator_toString(EagleDbSqlUnaryExpressionOperator op)
{
    switch(op) {
        
        case EagleDbSqlUnaryExpressionOperatorNegate:
            return strdup("-");
            
        case EagleDbSqlUnaryExpressionOperatorNot:
            return strdup("NOT");
            
    }
}
