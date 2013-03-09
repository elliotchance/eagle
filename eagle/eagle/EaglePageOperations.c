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
    int value = *((int*) obj), i, *destdata = (int*) destination->data, *source1data = (int*) source1->data;
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] < value);
    }
}

void EaglePageOperations_GreaterThanInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int value = *((int*) obj), i, *destdata = (int*) destination->data, *source1data = (int*) source1->data;
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] > value);
    }
}

void EaglePageOperations_AndPage(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i, *destdata = (int*) destination->data, *source1data = (int*) source1->data, *source2data = (int*) source2->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = source1data[i] && source2data[i];
    }
}

void EaglePageOperations_OrPage(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i, *destdata = (int*) destination->data, *source1data = (int*) source1->data, *source2data = (int*) source2->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = source1data[i] || source2data[i];
    }
}

void EaglePageOperations_NotPage(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i, *destdata = (int*) destination->data, *source1data = (int*) source1->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = !source1data[i];
    }
}

void EaglePageOperations_NegatePage(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i, *destdata = (int*) destination->data, *source1data = (int*) source1->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = -source1data[i];
    }
}

void EaglePageOperations_AdditionPage(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i, *destdata = (int*) destination->data, *source1data = (int*) source1->data, *source2data = (int*) source2->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = source1data[i] + source2data[i];
    }
}

void EaglePageOperations_MultiplyPage(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i, *destdata = (int*) destination->data, *source1data = (int*) source1->data, *source2data = (int*) source2->data;
    
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

void EaglePageOperations_EqualsPage(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i, *destdata = (int*) destination->data, *source1data = (int*) source1->data, *source2data = (int*) source2->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = source1data[i] == source2data[i];
    }
}

void EaglePageOperations_NotEqualsPage(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i, *destdata = (int*) destination->data, *source1data = (int*) source1->data, *source2data = (int*) source2->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = source1data[i] != source2data[i];
    }
}

void EaglePageOperations_ModulusPage(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i, *destdata = (int*) destination->data, *source1data = (int*) source1->data, *source2data = (int*) source2->data;
    
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

void EaglePageOperations_GreaterThanPage(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i, *destdata = (int*) destination->data, *source1data = (int*) source1->data, *source2data = (int*) source2->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] > source2data[i]);
    }
}

void EaglePageOperations_LessThanPage(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i, *destdata = (int*) destination->data, *source1data = (int*) source1->data, *source2data = (int*) source2->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] < source2data[i]);
    }
}

void EaglePageOperations_GreaterThanEqualPage(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i, *destdata = (int*) destination->data, *source1data = (int*) source1->data, *source2data = (int*) source2->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] >= source2data[i]);
    }
}

void EaglePageOperations_LessThanEqualPage(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i, *destdata = (int*) destination->data, *source1data = (int*) source1->data, *source2data = (int*) source2->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] <= source2data[i]);
    }
}

void EaglePageOperations_SubtractPage(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i, *destdata = (int*) destination->data, *source1data = (int*) source1->data, *source2data = (int*) source2->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = source1data[i] - source2data[i];
    }
}

void EaglePageOperations_DividePage(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i, *destdata = (int*) destination->data, *source1data = (int*) source1->data, *source2data = (int*) source2->data;
    
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

void EaglePageOperations_SqrtPage(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i, *destdata = (int*) destination->data, *source1data = (int*) source1->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (int) sqrt(source1data[i]);
    }
}
