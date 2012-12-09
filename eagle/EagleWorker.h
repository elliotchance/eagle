#ifndef eagle_EagleWorker_h
#define eagle_EagleWorker_h

typedef struct {
    int workerId;
} EagleWorker;

EagleWorker* EagleWorker_New(int workerId);

#endif
