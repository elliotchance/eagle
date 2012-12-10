#ifndef eagle_EaglePageOperations_h
#define eagle_EaglePageOperations_h

#define EaglePageOperationFunction(ptr) void (*ptr)(EaglePage *page, EaglePage *out, void *obj)

#include "EaglePage.h"

void EaglePageOperations_LessThanInt(EaglePage *page, EaglePage *out, void *obj);
void EaglePageOperations_GreaterThanInt(EaglePage *page, EaglePage *out, void *obj);

void EaglePageOperations_AndPage(EaglePage *page, EaglePage *out, void *obj);

#endif
