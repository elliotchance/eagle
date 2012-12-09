#ifndef eagle_EaglePageProvider_h
#define eagle_EaglePageProvider_h

#include "EaglePage.h"

struct EaglePageProvider_ {
    int offsetRecords;
    int totalRecords;
    int recordsPerPage;
    void *records;
    EaglePage* (*nextPage)(struct EaglePageProvider_ *epp);
    int (*pagesRemaining)(struct EaglePageProvider_ *epp);
    void *obj;
};
typedef struct EaglePageProvider_ EaglePageProvider;

EaglePageProvider* EaglePageProvider_New();
EaglePageProvider* EaglePageProvider_CreateFromIntStream(int *records, int totalRecords);
EaglePageProvider* EaglePageProvider_CreateFromPageReceiver(EaglePageReceiver *receiver);
int EaglePageProvider_TotalPages(int totalRecords, int recordsPerPage);

int EaglePageProvider_pagesRemaining(EaglePageProvider *epp);
EaglePage* EaglePageProvider_nextPage(EaglePageProvider *epp);

// private functions
int EaglePageProvider_pagesRemainingFromStream_(EaglePageProvider *epp);
int EaglePageProvider_pagesRemainingFromReceiver_(EaglePageProvider *epp);
EaglePage* EaglePageProvider_nextPageFromStream_(EaglePageProvider *epp);
EaglePage* EaglePageProvider_nextPageFromReceiver_(EaglePageProvider *epp);

#endif
