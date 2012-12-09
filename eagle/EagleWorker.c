#include <stdlib.h>
#include "EagleWorker.h"

EagleWorker* EagleWorker_New(int workerId)
{
    EagleWorker *worker = (EagleWorker*) malloc(sizeof(EagleWorker));
    worker->workerId = workerId;
    return worker;
}
