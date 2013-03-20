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
    int i;
    EagleDataTypeIntegerType value = *((EagleDataTypeIntegerType*) obj), *destdata = (EagleDataTypeIntegerType*) destination->data, *source1data = (EagleDataTypeIntegerType*) source1->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] < value);
    }
}

void EaglePageOperations_GreaterThanInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType value = *((EagleDataTypeIntegerType*) obj), *destdata = (EagleDataTypeIntegerType*) destination->data, *source1data = (EagleDataTypeIntegerType*) source1->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] > value);
    }
}

void EaglePageOperations_LessThanEqualsInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType value = *((EagleDataTypeIntegerType*) obj), *destdata = (EagleDataTypeIntegerType*) destination->data, *source1data = (EagleDataTypeIntegerType*) source1->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] <= value);
    }
}

void EaglePageOperations_GreaterThanEqualsInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType value = *((EagleDataTypeIntegerType*) obj), *destdata = (EagleDataTypeIntegerType*) destination->data, *source1data = (EagleDataTypeIntegerType*) source1->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] >= value);
    }
}

void EaglePageOperations_EqualsInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType value = *((EagleDataTypeIntegerType*) obj), *destdata = (EagleDataTypeIntegerType*) destination->data, *source1data = (EagleDataTypeIntegerType*) source1->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] == value);
    }
}

void EaglePageOperations_NotEqualsInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType value = *((EagleDataTypeIntegerType*) obj), *destdata = (EagleDataTypeIntegerType*) destination->data, *source1data = (EagleDataTypeIntegerType*) source1->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] != value);
    }
}

void EaglePageOperations_AdditionInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType value = *((EagleDataTypeIntegerType*) obj), *destdata = (EagleDataTypeIntegerType*) destination->data, *source1data = (EagleDataTypeIntegerType*) source1->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] + value);
    }
}

void EaglePageOperations_ModulusLeftInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType value = *((EagleDataTypeIntegerType*) obj), *destdata = (EagleDataTypeIntegerType*) destination->data, *source1data = (EagleDataTypeIntegerType*) source1->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        if(0 == value) {
            destdata[i] = 0;
        }
        else {
            destdata[i] = (source1data[i] % value);
        }
    }
}

void EaglePageOperations_ModulusRightInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType value = *((EagleDataTypeIntegerType*) obj), *destdata = (EagleDataTypeIntegerType*) destination->data, *source1data = (EagleDataTypeIntegerType*) source1->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        if(0 == source1data[i]) {
            destdata[i] = 0;
        }
        else {
            destdata[i] = (value % source1data[i]);
        }
    }
}

void EaglePageOperations_SubtractLeftInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType value = *((EagleDataTypeIntegerType*) obj), *destdata = (EagleDataTypeIntegerType*) destination->data, *source1data = (EagleDataTypeIntegerType*) source1->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] - value);
    }
}

void EaglePageOperations_SubtractRightInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType value = *((EagleDataTypeIntegerType*) obj), *destdata = (EagleDataTypeIntegerType*) destination->data, *source1data = (EagleDataTypeIntegerType*) source1->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (value - source1data[i]);
    }
}

void EaglePageOperations_MultiplyInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType value = *((EagleDataTypeIntegerType*) obj), *destdata = (EagleDataTypeIntegerType*) destination->data, *source1data = (EagleDataTypeIntegerType*) source1->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] * value);
    }
}

void EaglePageOperations_DivideLeftInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType value = *((EagleDataTypeIntegerType*) obj), *destdata = (EagleDataTypeIntegerType*) destination->data, *source1data = (EagleDataTypeIntegerType*) source1->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        if(0 == value) {
            destdata[i] = 0;
        }
        else {
            destdata[i] = (source1data[i] / value);
        }
    }
}

void EaglePageOperations_DivideRightInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType value = *((EagleDataTypeIntegerType*) obj), *destdata = (EagleDataTypeIntegerType*) destination->data, *source1data = (EagleDataTypeIntegerType*) source1->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        if(0 == source1data[i]) {
            destdata[i] = 0;
        }
        else {
            destdata[i] = (value / source1data[i]);
        }
    }
}

void EaglePageOperations_LessThanFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType *destdata = (EagleDataTypeIntegerType*) destination->data;
    EagleDataTypeFloatType value = *((EagleDataTypeFloatType*) obj), *source1data = (EagleDataTypeFloatType*) source1->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] < value);
    }
}

void EaglePageOperations_GreaterThanFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType *destdata = (EagleDataTypeIntegerType*) destination->data;
    EagleDataTypeFloatType value = *((EagleDataTypeFloatType*) obj), *source1data = (EagleDataTypeFloatType*) source1->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] > value);
    }
}

void EaglePageOperations_LessThanEqualsFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType *destdata = (EagleDataTypeIntegerType*) destination->data;
    EagleDataTypeFloatType value = *((EagleDataTypeFloatType*) obj), *source1data = (EagleDataTypeFloatType*) source1->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] <= value);
    }
}

void EaglePageOperations_GreaterThanEqualsFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType *destdata = (EagleDataTypeIntegerType*) destination->data;
    EagleDataTypeFloatType value = *((EagleDataTypeFloatType*) obj), *source1data = (EagleDataTypeFloatType*) source1->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] >= value);
    }
}

void EaglePageOperations_EqualsFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType *destdata = (EagleDataTypeIntegerType*) destination->data;
    EagleDataTypeFloatType value = *((EagleDataTypeFloatType*) obj), *source1data = (EagleDataTypeFloatType*) source1->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] == value);
    }
}

void EaglePageOperations_NotEqualsFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeIntegerType *destdata = (EagleDataTypeIntegerType*) destination->data;
    EagleDataTypeFloatType value = *((EagleDataTypeFloatType*) obj), *source1data = (EagleDataTypeFloatType*) source1->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] != value);
    }
}

void EaglePageOperations_AdditionFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeFloatType value = *((EagleDataTypeFloatType*) obj), *destdata = (EagleDataTypeFloatType*) destination->data, *source1data = (EagleDataTypeFloatType*) source1->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] + value);
    }
}

void EaglePageOperations_SubtractLeftFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeFloatType value = *((EagleDataTypeFloatType*) obj), *destdata = (EagleDataTypeFloatType*) destination->data, *source1data = (EagleDataTypeFloatType*) source1->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] - value);
    }
}

void EaglePageOperations_SubtractRightFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeFloatType value = *((EagleDataTypeFloatType*) obj), *destdata = (EagleDataTypeFloatType*) destination->data, *source1data = (EagleDataTypeFloatType*) source1->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (value - source1data[i]);
    }
}

void EaglePageOperations_MultiplyFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EagleDataTypeFloatType value = *((EagleDataTypeFloatType*) obj), *destdata = (EagleDataTypeFloatType*) destination->data, *source1data = (EagleDataTypeFloatType*) source1->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] * value);
    }
}

void EaglePageOperations_DivideLeftFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    EagleDataTypeFloatType value = *((EagleDataTypeFloatType*) obj), *destdata = (EagleDataTypeFloatType*) destination->data, *source1data = (EagleDataTypeFloatType*) source1->data;
    int i;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        if(0 == value) {
            destdata[i] = 0;
        }
        else {
            destdata[i] = (source1data[i] / value);
        }
    }
}

void EaglePageOperations_DivideRightFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    EagleDataTypeFloatType value = *((EagleDataTypeFloatType*) obj), *destdata = (EagleDataTypeFloatType*) destination->data, *source1data = (EagleDataTypeFloatType*) source1->data;
    int i;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        if(0 == source1data[i]) {
            destdata[i] = 0;
        }
        else {
            destdata[i] = (value / source1data[i]);
        }
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
    
    switch(provider->type) {
            
        case EagleDataTypeUnknown:
            EagleLogger_Log(EagleLoggerSeverityError, "Unknown type.");
            return;
            
        case EagleDataTypeInteger:
        {
            EagleDataTypeIntegerType *source1data;
            EagleDataTypeIntegerType *source2data = (EagleDataTypeIntegerType*) source2->data;
            
            /* send all */
            if(NULL == source1) {
                for(i = 0; i < source2->count; ++i) {
                    EaglePageProvider_add(provider, &source2data[i]);
                }
                break;
            }
            
            /* send some */
            source1data = (EagleDataTypeIntegerType*) source1->data;
            for(i = 0; i < source2->count; ++i) {
                if(source1data[i]) {
                    EaglePageProvider_add(provider, &source2data[i]);
                }
            }
            break;
        }
            
        case EagleDataTypeVarchar:
        {
            EagleDataTypeIntegerType *source1data;
            EagleDataTypeVarcharType *source2data = (EagleDataTypeVarcharType*) source2->data;
            
            /* send all */
            if(NULL == source1) {
                for(i = 0; i < source2->count; ++i) {
                    EaglePageProvider_add(provider, strdup(source2data[i]));
                }
                break;
            }
            
            /* send some */
            source1data = (EagleDataTypeIntegerType*) source1->data;
            for(i = 0; i < source2->count; ++i) {
                if(source1data[i]) {
                    EaglePageProvider_add(provider, strdup(source2data[i]));
                }
            }
            break;
        }
            
        case EagleDataTypeFloat:
        {
            EagleDataTypeIntegerType *source1data;
            EagleDataTypeFloatType *source2data = (EagleDataTypeFloatType*) source2->data;
            
            /* send all */
            if(NULL == source1) {
                for(i = 0; i < source2->count; ++i) {
                    EaglePageProvider_add(provider, &source2data[i]);
                }
                break;
            }
            
            /* send some */
            source1data = (EagleDataTypeIntegerType*) source1->data;
            for(i = 0; i < source2->count; ++i) {
                if(source1data[i]) {
                    EaglePageProvider_add(provider, &source2data[i]);
                }
            }
            break;
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
