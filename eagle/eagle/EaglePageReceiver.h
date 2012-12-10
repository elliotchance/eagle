#ifndef eagle_EaglePageReceiver_h
#define eagle_EaglePageReceiver_h

#include <pthread.h>

typedef struct {
    int allocated;
    int used;
    int *buffer;
    pthread_mutex_t *pushRecordIdLock;
} EaglePageReceiver;

EaglePageReceiver* EaglePageReceiver_New(void);
void EaglePageReceiver_pushRecordId(EaglePageReceiver *pr, int recordId);

#endif
