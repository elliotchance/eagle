#ifndef eagle_EagleObject_h
#define eagle_EagleObject_h

#include "Eagle.h"
#include "EagleBoolean.h"

/**
 Check if two objects are equal. If either of the objects are NULL or both of them are NULL then the result will be
 EagleFalse.
 @param [in] obj1 First object.
 @param [in] obj2 Second object.
 @return EagleTrue if both parameters are not NULL and they have the same pointer address.
 */
EagleBoolean EagleObject_equals(void *obj1, void *obj2);

#endif
