#include <stdlib.h>
#include "EaglePage.h"

EaglePage* EaglePage_New(int *data, int count, int recordOffset)
{
    EaglePage *page = (EaglePage*) malloc(sizeof(EaglePage));
    
    page->data = data;
    page->count = count;
    page->recordOffset = recordOffset;
    
    return page;
}

EaglePage* EaglePage_Alloc(int count)
{
    int *data = (int*) calloc(count, sizeof(int));
    return EaglePage_New(data, count, 0);
}
