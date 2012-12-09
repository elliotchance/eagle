#ifndef eagle_EagleWorkers_h
#define eagle_EagleWorkers_h

#include "EagleWorker.h"

typedef struct {
    EagleWorker **workers;
    int totalWorkers;
} EagleWorkers;

EagleWorkers* EagleWorkers_New(int totalWorkers);

#endif
