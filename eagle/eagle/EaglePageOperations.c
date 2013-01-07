#include <stdlib.h>
#include <stdio.h>
#include "EaglePageOperations.h"
#include "EaglePageProvider.h"
#include "EagleUtils.h"
#include "EagleLogger.h"

#include <unistd.h>

/**
 Calculate values in a page that are less than an integer.
 
 This function does not allocate memory.
 
 @param [in] destination Must be preallocated to the same size as \p page. All records in this page will be given a
        value or 0 or 1 based on the result of the comparison.
 @param [in] source1 The page to scan, can be any size.
 @param [in] source2 Ignored.
 @param [in] obj Must be int* which is dereferenced to the integer value for comparison.
 
 @see EagleData_Int()
 */
void EaglePageOperations_LessThanInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int value = *((int*) obj), i, *destdata = (int*) destination->data, *source1data = (int*) source1->data;
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] < value);
    }
}

/**
 Calculate values in a page that are greater than an integer.
 
 This function does not allocate memory.
 
 @param [in] destination Must be preallocated to the same size as \p page. All records in this page will be given a
        value or 0 or 1 based on the result of the comparison.
 @param [in] source1 The page to scan, can be any size.
 @param [in] source2 Ignored.
 @param [in] obj Must be int* which is dereferenced to the integer value for comparison.
 
 @see EagleData_Int()
 */
void EaglePageOperations_GreaterThanInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int value = *((int*) obj), i, *destdata = (int*) destination->data, *source1data = (int*) source1->data;
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] > value);
    }
}

/**
 Calculate logical AND between two pages.
 
 @param [in] destination Must be preallocated to the same size as \p page. All records in this page will be given a
        value or 0 or 1 based on the result of the comparison.
 @param [in] source1 Left operand.
 @param [in] source2 Right operand.
 @param [in] obj Ignored.
 */
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

void EaglePageOperations_CastIntPageToBoolean(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i, *destdata = (int*) destination->data, *source1data = (int*) source1->data;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destdata[i] = (source1data[i] != 0);
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
        destdata[i] = source1data[i] % source2data[i];
    }
}
