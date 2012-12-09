#include <stdlib.h>
#include "EagleWorkers.h"

EagleWorkers* EagleWorkers_New(int totalWorkers)
{
    EagleWorkers *workers = (EagleWorkers*) malloc(sizeof(EagleWorkers));
    workers->workers = (EagleWorker**) calloc(sizeof(EagleWorker*), totalWorkers);
    for(int i = 0; i < totalWorkers; ++i) {
        workers->workers[i] = EagleWorker_New(i);
    }
    workers->totalWorkers = totalWorkers;
    return workers;
}
