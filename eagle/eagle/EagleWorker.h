#ifndef eagle_EagleWorker_h
#define eagle_EagleWorker_h

#include <pthread.h>

struct EagleInstance_;

typedef struct {
    int workerId;
    pthread_t thread;
    struct EagleInstance_ *instance;
} EagleWorker;

EagleWorker* EagleWorker_New(int workerId, struct EagleInstance_ *instance);

/**
 This does not release the \c instance as that's only a reference to the parent object.
 */
void EagleWorker_Delete(EagleWorker *worker);

void EagleWorker_start(EagleWorker *worker);

void* EagleWorker_begin(void *obj);

void EagleWorker_join(EagleWorker *worker);

#endif
