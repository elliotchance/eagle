#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "EagleDbSqlBinaryExpressionOperator.h"

char* EagleDbSqlBinaryExpressionOperator_toString(EagleDbSqlBinaryExpressionOperator op)
{
    switch(op) {
            
        case EagleDbSqlBinaryExpressionOperatorPlus:
            return strdup("+");
            
        case EagleDbSqlBinaryExpressionOperatorEquals:
            return strdup("=");
            
        case EagleDbSqlBinaryExpressionOperatorModulus:
            return strdup("%");
            
        case EagleDbSqlBinaryExpressionOperatorMultiply:
            return strdup("*");
            
        case EagleDbSqlBinaryExpressionOperatorNotEquals:
            return strdup("!=");
            
        case EagleDbSqlBinaryExpressionOperatorGreaterThan:
            return strdup(">");
            
        case EagleDbSqlBinaryExpressionOperatorLessThan:
            return strdup("<");
            
        case EagleDbSqlBinaryExpressionOperatorGreaterThanEqual:
            return strdup(">=");
            
        case EagleDbSqlBinaryExpressionOperatorLessThanEqual:
            return strdup("<=");
            
        case EagleDbSqlBinaryExpressionOperatorMinus:
            return strdup("-");
            
        case EagleDbSqlBinaryExpressionOperatorDivide:
            return strdup("/");
            
        case EagleDbSqlBinaryExpressionOperatorOr:
            return strdup("OR");
            
        case EagleDbSqlBinaryExpressionOperatorAnd:
            return strdup("AND");
            
    }
}
