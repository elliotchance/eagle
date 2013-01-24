#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "EagleData.h"
#include "EagleUtils.h"
#include "EagleMemory.h"

int* EagleData_Int(int value)
{
    int *r = (int*) EagleMemory_Allocate("EagleData_Int.1", sizeof(int));
    if(NULL == r) {
        return NULL;
    }
    *r = value;
    return r;
}
