#include <stdlib.h>
#include "EagleData.h"

/**
 Allocate an integer and return the pointer.
 @param value Value to set into newly allocated space.
 */
int* EagleData_Int(int value)
{
    int *r = (int*) malloc(sizeof(int));
    *r = value;
    return r;
}
