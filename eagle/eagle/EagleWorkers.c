#include <stdlib.h>
#include "EagleWorkers.h"

EagleWorkers* EagleWorkers_New(int totalWorkers, struct EagleInstance_ *instance)
{
    EagleWorkers *workers = (EagleWorkers*) malloc(sizeof(EagleWorkers));
    int i;
    
    workers->workers = (EagleWorker**) calloc((size_t) totalWorkers, sizeof(EagleWorker*));
    for(i = 0; i < totalWorkers; ++i) {
        workers->workers[i] = EagleWorker_New(i, instance);
    }
    workers->totalWorkers = totalWorkers;
    return workers;
}

void EagleWorkers_start(EagleWorkers *workers)
{
    int i;
    
    for(i = 0; i < workers->totalWorkers; ++i) {
        EagleWorker_start(workers->workers[i]);
    }
}

void EagleWorkers_joinAll(EagleWorkers *workers)
{
    int i;
    
    for(i = 0; i < workers->totalWorkers; ++i) {
        EagleWorker_join(workers->workers[i]);
    }
}

void EagleWorkers_Delete(EagleWorkers *workers)
{
    int i;
    
    for(i = 0; i < workers->totalWorkers; ++i) {
        EagleWorker_Delete(workers->workers[i]);
    }
    free((void*) workers->workers);
    
    free((void*) workers);
}
