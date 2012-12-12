#include <stdlib.h>
#include <stdio.h>
#include "EaglePageOperations.h"
#include "EaglePageProvider.h"

#include <unistd.h>

/**
 Calculate values in a page that are less than an integer.
 
 This function does not allocate memory.
 
 @param page The page to scan, can be any size.
 @param out Must be preallocated to the same size as \p page. All records in this page will be given a value or 0 or 1
        based on the result of the comparison.
 @param obj Must be int* which is dereferenced to the integer value for comparison.
 
 @see EagleData_Int()
 */
void EaglePageOperations_LessThanInt(EaglePage *page, EaglePage *out, void *obj)
{
    int value = *((int*) obj), i;
    out->recordOffset = page->recordOffset;
    out->count = page->count;
    
    for(i = 0; i < page->count; ++i) {
        out->data[i] = (page->data[i] < value);
    }
}

/**
 Calculate values in a page that are greater than an integer.
 
 This function does not allocate memory.
 
 @param page The page to scan, can be any size.
 @param out Must be preallocated to the same size as \p page. All records in this page will be given a value or 0 or 1
        based on the result of the comparison.
 @param obj Must be int* which is dereferenced to the integer value for comparison.
 
 @see EagleData_Int()
 */
void EaglePageOperations_GreaterThanInt(EaglePage *page, EaglePage *out, void *obj)
{
    int value = *((int*) obj), i;
    out->recordOffset = page->recordOffset;
    out->count = page->count;
    
    for(i = 0; i < page->count; ++i) {
        out->data[i] = (page->data[i] > value);
    }
}

/**
 Calculate logical AND between two pages.
 
 @param page The page to scan, can be any size.
 @param out Must be preallocated to the same size as \p page. All records in this page will be given a value or 0 or 1
        based on the result of the comparison.
 @param obj Must be EaglePage* which becomes the right operand for \p page to calculate logical AND against. This page
        must also be the same size (or greater) of \p page and \p out
 */
void EaglePageOperations_AndPage(EaglePage *page, EaglePage *out, void *obj)
{
    EaglePage *page2 = (EaglePage*) obj;
    int i;
    
    out->recordOffset = page->recordOffset;
    out->count = page->count;
    
    for(i = 0; i < page->count; ++i) {
        out->data[i] = page->data[i] && page2->data[i];
    }
}

void EaglePageOperations_AdditionPage(EaglePage *page, EaglePage *out, void *obj)
{
    EaglePage *page2 = (EaglePage*) obj;
    int i;
    
    out->recordOffset = page->recordOffset;
    out->count = page->count;
    
    for(i = 0; i < page->count; ++i) {
        out->data[i] = page->data[i] + page2->data[i];
    }
}

void EaglePageOperations_CastIntPageToBoolean(EaglePage *page, EaglePage *out, void *obj)
{
    int i;
    
    out->recordOffset = page->recordOffset;
    out->count = page->count;
    
    for(i = 0; i < page->count; ++i) {
        out->data[i] = (page->data[i] != 0);
    }
}
