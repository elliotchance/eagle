#include <stdlib.h>
#include <stdio.h>
#include "EaglePageOperations.h"
#include "EaglePageProvider.h"

#include <unistd.h>

/**
 Calculate values in a page that are less than an integer.
 
 This function does not allocate memory.
 
 @param destination Must be preallocated to the same size as \p page. All records in this page will be given a value or
        0 or 1 based on the result of the comparison.
 @param source1 The page to scan, can be any size.
 @param source2 Ignored.
 @param obj Must be int* which is dereferenced to the integer value for comparison.
 
 @see EagleData_Int()
 */
void EaglePageOperations_LessThanInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int value = *((int*) obj), i;
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destination->data[i] = (source1->data[i] < value);
    }
}

/**
 Calculate values in a page that are greater than an integer.
 
 This function does not allocate memory.
 
 @param destination Must be preallocated to the same size as \p page. All records in this page will be given a value or
        0 or 1 based on the result of the comparison.
 @param source1 The page to scan, can be any size.
 @param source2 Ignored.
 @param obj Must be int* which is dereferenced to the integer value for comparison.
 
 @see EagleData_Int()
 */
void EaglePageOperations_GreaterThanInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int value = *((int*) obj), i;
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destination->data[i] = (source1->data[i] > value);
    }
}

/**
 Calculate logical AND between two pages.
 
 @param destination Must be preallocated to the same size as \p page. All records in this page will be given a value or
        0 or 1 based on the result of the comparison.
 @param source1 Left operand.
 @param source2 Right operand.
 @param obj Ignored.
 */
void EaglePageOperations_AndPage(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destination->data[i] = source1->data[i] && source2->data[i];
    }
}

void EaglePageOperations_AdditionPage(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destination->data[i] = source1->data[i] + source2->data[i];
    }
}

void EaglePageOperations_CastIntPageToBoolean(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    
    destination->recordOffset = source1->recordOffset;
    destination->count = source1->count;
    
    for(i = 0; i < source1->count; ++i) {
        destination->data[i] = (source1->data[i] != 0);
    }
}

void EaglePageOperations_SendIntPageToProvider(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)
{
    int i;
    EaglePageProvider *provider = (EaglePageProvider*) obj;
    
    for(i = 0; i < source1->count; ++i) {
        EaglePageProvider_add(provider, (void*) source1->data[i]);
    }
}
