#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "EagleData.h"
#include "EagleUtils.h"
#include "EagleMemory.h"

EagleDataTypeIntegerType* EagleData_Int(EagleDataTypeIntegerType value)
{
    EagleDataTypeIntegerType *r = (EagleDataTypeIntegerType*) EagleMemory_Allocate("EagleData_Int.1", sizeof(EagleDataTypeIntegerType));
    if(NULL == r) {
        return NULL;
    }
    
    *r = value;
    return r;
}

EagleDataTypeFloatType* EagleData_Float(EagleDataTypeFloatType value)
{
    EagleDataTypeFloatType *r = (EagleDataTypeFloatType*) EagleMemory_Allocate("EagleData_Float.1", sizeof(EagleDataTypeFloatType));
    if(NULL == r) {
        return NULL;
    }
    
    *r = value;
    return r;
}
