#include <stdlib.h>
#include "EaglePage.h"

/**
 Create a new EaglePage.
 You will be required to provide a data block with the number of records in that data block.
 
 @param data The preallocated data block. This may be NULL, but the \p count must be 0.
 @param count The number of records in the data block.
 @param recordOffset An arbitrary number stored in the page. This number is used as an offset when calculating the
        record ID of a given record inside a page. If you are unsure as to what this value should be then you can use 0.
 */
EaglePage* EaglePage_New(int *data, int count, int recordOffset)
{
    EaglePage *page = (EaglePage*) malloc(sizeof(EaglePage));
    
    page->data = data;
    page->count = count;
    page->recordOffset = recordOffset;
    
    return page;
}

/**
 Allocate a new page of given size.
 This function differs from EaglePage_New() because it will allocate the data block for you.
 
 @note The page allocated will not be filled with any data, make sure you sanitise the page if you intend to read from
       it.
 
 @param count The number of records of the page.
 */
EaglePage* EaglePage_Alloc(int count)
{
    int *data = (int*) calloc((size_t) count, sizeof(int));
    return EaglePage_New(data, count, 0);
}
