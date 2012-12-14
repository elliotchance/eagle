#ifndef eagle_EagleWorkers_h
#define eagle_EagleWorkers_h

#include "EagleWorker.h"

struct EagleInstance_;

typedef struct {
    EagleWorker **workers;
    int totalWorkers;
} EagleWorkers;

EagleWorkers* EagleWorkers_New(int totalWorkers, struct EagleInstance_ *instance);
void EagleWorkers_Delete(EagleWorkers *workers);

void EagleWorkers_start(EagleWorkers *workers);
void EagleWorkers_joinAll(EagleWorkers *workers);

#endif
