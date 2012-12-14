#ifndef eagle_EaglePage_h
#define eagle_EaglePage_h

struct EaglePage_ {
    int recordOffset;
    int count;
    int *data;
};
typedef struct EaglePage_ EaglePage;

EaglePage* EaglePage_New(int *data, int count, int recordOffset);
EaglePage* EaglePage_Alloc(int count);
void EaglePage_Delete(EaglePage *page);

#endif
