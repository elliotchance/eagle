#include <stdlib.h>
#include "EagleData.h"

int* EagleData_Int(int value)
{
    int *r = (int*) malloc(sizeof(int));
    *r = value;
    return r;
}
