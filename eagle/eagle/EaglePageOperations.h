#ifndef eagle_EaglePageOperations_h
#define eagle_EaglePageOperations_h

#define EaglePageOperationFunction(ptr) void (*ptr)(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)

#include "EaglePage.h"

void EaglePageOperations_LessThanInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);
void EaglePageOperations_GreaterThanInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);
void EaglePageOperations_CastIntToBool(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

void EaglePageOperations_AndPage(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);
void EaglePageOperations_AdditionPage(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);
void EaglePageOperations_EqualsPage(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);
void EaglePageOperations_CastIntPageToBoolean(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);
void EaglePageOperations_SendIntPageToProvider(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);
void EaglePageOperations_ModulusPage(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

#endif
