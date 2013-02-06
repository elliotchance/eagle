#include <stdlib.h>
#include <stdio.h>
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

void EaglePageOperations_AdditionPage(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i, *destdata = (int*) destination->data, *source1data = (int*) source1->data, *source2data = (int*) source2->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = source1data[i] + source2data[i];
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
                    int *source2data = (int*) source2->data;
                    EaglePageProvider_add(provider, &source2data[i]);
                    break;
                }
                    
                case EagleDataTypeText:
                {
                    char **source2data = (char**) source2->data;
                    EaglePageProvider_add(provider, source2data[i]);
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
