#include <stdlib.h>
#include <stdio.h>
#include "EaglePageOperations.h"
#include "EaglePageProvider.h"

void EaglePageOperations_LessThanInt(EaglePage *page, EaglePage *out, void *obj)
{
    int value = *((int*) obj);
    
    // scan page
    for(int i = 0; i < page->count; ++i) {
        out->data[i] = (page->data[i] < value);
    }
}

void EaglePageOperations_GreaterThanInt(EaglePage *page, EaglePage *out, void *obj)
{
    int value = *((int*) obj);
    
    // scan page
    for(int i = 0; i < page->count; ++i) {
        out->data[i] = (page->data[i] > value);
    }
}

void EaglePageOperations_AndPage(EaglePage *page, EaglePage *out, void *obj)
{
    EaglePage *page2 = (EaglePage*) obj;
    
    // compare pages
    for(int i = 0; i < page->count; ++i) {
        out->data[i] = page->data[i] && page2->data[i];
    }
}
