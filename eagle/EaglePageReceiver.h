#ifndef eagle_EaglePageReceiver_h
#define eagle_EaglePageReceiver_h

typedef struct {
    int allocated;
    int used;
    int *buffer;
} EaglePageReceiver;

EaglePageReceiver* EaglePageReceiver_New(int *data, int count);

void EaglePageReceiver_pushInt(EaglePageReceiver *pr, int value);

#endif
