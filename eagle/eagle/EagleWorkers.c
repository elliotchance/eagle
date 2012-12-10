#include <stdlib.h>
#include "EagleWorkers.h"

EagleWorkers* EagleWorkers_New(int totalWorkers, struct EagleInstance_ *instance)
{
    EagleWorkers *workers = (EagleWorkers*) malloc(sizeof(EagleWorkers));
    workers->workers = (EagleWorker**) calloc((size_t) totalWorkers, sizeof(EagleWorker*));
    for(int i = 0; i < totalWorkers; ++i) {
        workers->workers[i] = EagleWorker_New(i, instance);
    }
    workers->totalWorkers = totalWorkers;
    return workers;
}

void EagleWorkers_start(EagleWorkers *workers)
{
    for(int i = 0; i < workers->totalWorkers; ++i) {
        EagleWorker_start(workers->workers[i]);
    }
}

void EagleWorkers_joinAll(EagleWorkers *workers)
{
    for(int i = 0; i < workers->totalWorkers; ++i) {
        EagleWorker_join(workers->workers[i]);
    }
}
