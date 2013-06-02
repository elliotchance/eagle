#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "EagleObject.h"

EagleBoolean EagleObject_equals(void *obj1, void *obj2)
{
    /* check for NULLs */
    if(NULL == obj1 || NULL == obj2) {
        return EagleFalse;
    }
        
    /* if the pointers are equal then we don't need to go any further */
    return obj1 == obj2;
}
