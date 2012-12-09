#ifndef eagle_EaglePageReceiver_h
#define eagle_EaglePageReceiver_h

typedef struct {
    int allocated;
    int used;
    int *buffer;
} EaglePageReceiver;

EaglePageReceiver* EaglePageReceiver_New();

void EaglePageReceiver_pushRecordId(EaglePageReceiver *pr, int recordId);

#endif
