#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "EaglePageOperations.h"
#include "EaglePageProvider.h"
#include "EagleUtils.h"
#include "EagleLogger.h"

#include <unistd.h>

void EaglePageOperations_LessThanInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int value = *((EagleDataTypeIntegerType*) obj), i, *destdata = (EagleDataTypeIntegerType*) destination->data, *source1data = (EagleDataTypeIntegerType*) source1->data;
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] < value);
    }
}

void EaglePageOperations_GreaterThanInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int value = *((EagleDataTypeIntegerType*) obj), i, *destdata = (EagleDataTypeIntegerType*) destination->data, *source1data = (EagleDataTypeIntegerType*) source1->data;
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] > value);
    }
}

void EaglePageOperations_AndPageInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType *destdata = (EagleDataTypeIntegerType*) destination->data, *source1data = (EagleDataTypeIntegerType*) source1->data, *source2data = (EagleDataTypeIntegerType*) source2->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = source1data[i] && source2data[i];
    }
}

void EaglePageOperations_OrPageInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType *destdata = (EagleDataTypeIntegerType*) destination->data, *source1data = (EagleDataTypeIntegerType*) source1->data, *source2data = (EagleDataTypeIntegerType*) source2->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = source1data[i] || source2data[i];
    }
}

void EaglePageOperations_NotPageInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType *destdata = (EagleDataTypeIntegerType*) destination->data, *source1data = (EagleDataTypeIntegerType*) source1->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = !source1data[i];
    }
}

void EaglePageOperations_NegatePageInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType *destdata = (EagleDataTypeIntegerType*) destination->data, *source1data = (EagleDataTypeIntegerType*) source1->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = -source1data[i];
    }
}

void EaglePageOperations_CastPageIntFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeFloatType *destdata = (EagleDataTypeFloatType*) destination->data;
    EagleDataTypeIntegerType *source1data = (EagleDataTypeIntegerType*) source1->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (EagleDataTypeFloatType) source1data[i];
    }
}

void EaglePageOperations_NegatePageFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeFloatType *destdata = (EagleDataTypeFloatType*) destination->data, *source1data = (EagleDataTypeFloatType*) source1->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = -source1data[i];
    }
}

void EaglePageOperations_AdditionPageInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType *destdata = (EagleDataTypeIntegerType*) destination->data, *source1data = (EagleDataTypeIntegerType*) source1->data, *source2data = (EagleDataTypeIntegerType*) source2->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = source1data[i] + source2data[i];
    }
}

void EaglePageOperations_AdditionPageFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeFloatType *destdata = (EagleDataTypeFloatType*) destination->data, *source1data = (EagleDataTypeFloatType*) source1->data, *source2data = (EagleDataTypeFloatType*) source2->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = source1data[i] + source2data[i];
    }
}

void EaglePageOperations_MultiplyPageInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType *destdata = (EagleDataTypeIntegerType*) destination->data, *source1data = (EagleDataTypeIntegerType*) source1->data, *source2data = (EagleDataTypeIntegerType*) source2->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = source1data[i] * source2data[i];
    }
}

void EaglePageOperations_MultiplyPageFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeFloatType *destdata = (EagleDataTypeFloatType*) destination->data, *source1data = (EagleDataTypeFloatType*) source1->data, *source2data = (EagleDataTypeFloatType*) source2->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = source1data[i] * source2data[i];
    }
}

void EaglePageOperations_SendPageToProvider(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EaglePageProvider *provider = (EaglePageProvider*) obj;
    
    for(i = 0; i < source2->count; ++i) {
        if(source1 == NULL || ((int*) source1->data)[i]) {
            switch(provider->type) {
                    
                case EagleDataTypeUnknown:
                    EagleLogger_Log(EagleLoggerSeverityError, "Unknown type.");
                    return;
                    
                case EagleDataTypeInteger:
                {
                    EagleDataTypeIntegerType *source2data = (EagleDataTypeIntegerType*) source2->data;
                    EaglePageProvider_add(provider, &source2data[i]);
                    break;
                }
                    
                case EagleDataTypeVarchar:
                {
                    EagleDataTypeVarcharType *source2data = (EagleDataTypeVarcharType*) source2->data;
                    EaglePageProvider_add(provider, strdup(source2data[i]));
                    break;
                }
                    
                case EagleDataTypeFloat:
                {
                    EagleDataTypeFloatType *source2data = (EagleDataTypeFloatType*) source2->data;
                    EaglePageProvider_add(provider, &source2data[i]);
                    break;
                }
                
            }
        }
    }
}

void EaglePageOperations_EqualsPageInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType *destdata = (EagleDataTypeIntegerType*) destination->data, *source1data = (EagleDataTypeIntegerType*) source1->data, *source2data = (EagleDataTypeIntegerType*) source2->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = source1data[i] == source2data[i];
    }
}

void EaglePageOperations_EqualsPageFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType *destdata = (EagleDataTypeIntegerType*) destination->data;
    EagleDataTypeFloatType *source1data = (EagleDataTypeFloatType*) source1->data, *source2data = (EagleDataTypeFloatType*) source2->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = source1data[i] == source2data[i];
    }
}

void EaglePageOperations_NotEqualsPageInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType *destdata = (EagleDataTypeIntegerType*) destination->data, *source1data = (EagleDataTypeIntegerType*) source1->data, *source2data = (EagleDataTypeIntegerType*) source2->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = source1data[i] != source2data[i];
    }
}

void EaglePageOperations_NotEqualsPageFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType *destdata = (EagleDataTypeIntegerType*) destination->data;
    EagleDataTypeFloatType *source1data = (EagleDataTypeFloatType*) source1->data, *source2data = (EagleDataTypeFloatType*) source2->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = source1data[i] != source2data[i];
    }
}

void EaglePageOperations_ModulusPageInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType *destdata = (EagleDataTypeIntegerType*) destination->data, *source1data = (EagleDataTypeIntegerType*) source1->data, *source2data = (EagleDataTypeIntegerType*) source2->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        if(0 == source2data[i]) {
            destdata[i] = 0;
        }
        else {
            destdata[i] = source1data[i] % source2data[i];
        }
    }
}

void EaglePageOperations_GreaterThanPageInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType *destdata = (EagleDataTypeIntegerType*) destination->data, *source1data = (EagleDataTypeIntegerType*) source1->data, *source2data = (EagleDataTypeIntegerType*) source2->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] > source2data[i]);
    }
}

void EaglePageOperations_GreaterThanPageFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType *destdata = (EagleDataTypeIntegerType*) destination->data;
    EagleDataTypeFloatType *source1data = (EagleDataTypeFloatType*) source1->data, *source2data = (EagleDataTypeFloatType*) source2->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] > source2data[i]);
    }
}

void EaglePageOperations_LessThanPageInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType *destdata = (EagleDataTypeIntegerType*) destination->data, *source1data = (EagleDataTypeIntegerType*) source1->data, *source2data = (EagleDataTypeIntegerType*) source2->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] < source2data[i]);
    }
}

void EaglePageOperations_LessThanPageFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType *destdata = (EagleDataTypeIntegerType*) destination->data;
    EagleDataTypeFloatType *source1data = (EagleDataTypeFloatType*) source1->data, *source2data = (EagleDataTypeFloatType*) source2->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] < source2data[i]);
    }
}

void EaglePageOperations_GreaterThanEqualPageInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType *destdata = (EagleDataTypeIntegerType*) destination->data, *source1data = (EagleDataTypeIntegerType*) source1->data, *source2data = (EagleDataTypeIntegerType*) source2->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] >= source2data[i]);
    }
}

void EaglePageOperations_GreaterThanEqualPageFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType *destdata = (EagleDataTypeIntegerType*) destination->data;
    EagleDataTypeFloatType *source1data = (EagleDataTypeFloatType*) source1->data, *source2data = (EagleDataTypeFloatType*) source2->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] >= source2data[i]);
    }
}

void EaglePageOperations_LessThanEqualPageInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType *destdata = (EagleDataTypeIntegerType*) destination->data, *source1data = (EagleDataTypeIntegerType*) source1->data, *source2data = (EagleDataTypeIntegerType*) source2->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] <= source2data[i]);
    }
}

void EaglePageOperations_LessThanEqualPageFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType *destdata = (EagleDataTypeIntegerType*) destination->data;
    EagleDataTypeFloatType *source1data = (EagleDataTypeFloatType*) source1->data, *source2data = (EagleDataTypeFloatType*) source2->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] <= source2data[i]);
    }
}

void EaglePageOperations_SubtractPageInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType *destdata = (EagleDataTypeIntegerType*) destination->data, *source1data = (EagleDataTypeIntegerType*) source1->data, *source2data = (EagleDataTypeIntegerType*) source2->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = source1data[i] - source2data[i];
    }
}

void EaglePageOperations_SubtractPageFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeFloatType *destdata = (EagleDataTypeFloatType*) destination->data, *source1data = (EagleDataTypeFloatType*) source1->data, *source2data = (EagleDataTypeFloatType*) source2->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = source1data[i] - source2data[i];
    }
}

void EaglePageOperations_DividePageInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType *destdata = (EagleDataTypeIntegerType*) destination->data, *source1data = (EagleDataTypeIntegerType*) source1->data, *source2data = (EagleDataTypeIntegerType*) source2->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        if(0 == source2data[i]) {
            destdata[i] = 0;
        }
        else {
            destdata[i] = source1data[i] / source2data[i];
        }
    }
}

void EaglePageOperations_DividePageFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeFloatType *destdata = (EagleDataTypeFloatType*) destination->data, *source1data = (EagleDataTypeFloatType*) source1->data, *source2data = (EagleDataTypeFloatType*) source2->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        if(0 == source2data[i]) {
            destdata[i] = 0;
        }
        else {
            destdata[i] = source1data[i] / source2data[i];
        }
    }
}

void EaglePageOperations_SqrtPageFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeFloatType *destdata = (EagleDataTypeFloatType*) destination->data, *source1data = (EagleDataTypeFloatType*) source1->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (EagleDataTypeFloatType) sqrt(source1data[i]);
    }
}
