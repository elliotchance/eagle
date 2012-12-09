#ifndef eagle_EaglePage_h
#define eagle_EaglePage_h

#include "EaglePageReceiver.h"

struct EaglePage_ {
    int count;
    int *data;
};
typedef struct EaglePage_ EaglePage;

EaglePage* EaglePage_New(int *data, int count);
EaglePage* EaglePage_Alloc(int count);

#endif
