#include <stdlib.h>
#include "EagleWorkers.h"
#include "EagleMemory.h"

EagleWorkers* EagleWorkers_New(int totalWorkers, struct EagleInstance_ *instance)
{
    EagleWorkers *workers = (EagleWorkers*) EagleMemory_Allocate("EagleWorkers_New.1", sizeof(EagleWorkers));
    int i;
    
    if(NULL == workers) {
        return NULL;
    }
    
    workers->workers = (EagleWorker**) EagleMemory_MultiAllocate("EagleWorkers_New.2", sizeof(EagleWorker*), totalWorkers);
    if(NULL == workers->workers) {
        EagleMemory_Free(workers);
        return NULL;
    }
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
    EagleMemory_Free((void*) workers->workers);
    
    EagleMemory_Free((void*) workers);
}
