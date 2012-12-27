#include <stdio.h>
#include <stdlib.h>
#include "EagleDbSqlExpressionOperator.h"

char* EagleDbSqlExpressionOperator_toString(EagleDbSqlExpressionOperator op)
{
    switch(op) {
        case EagleDbSqlExpressionOperatorPlus:
            return "+";
            
        case EagleDbSqlExpressionOperatorEquals:
            return "=";
            
        case EagleDbSqlExpressionOperatorModulus:
            return "%";
    }
}
