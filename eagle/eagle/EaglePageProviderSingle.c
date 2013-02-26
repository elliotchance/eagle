#include <string.h>
#include "EaglePageProviderSingle.h"
#include "EagleMemory.h"
#include "EaglePageProviderArray.h"

EaglePageProviderSingle* EaglePageProviderSingle_NewInt(int value, int recordsPerPage, char *name)
{
    EaglePageProviderSingle *pageProvider = (EaglePageProviderSingle*) EagleMemory_Allocate("EaglePageProviderSingle_NewInt.1", sizeof(EaglePageProviderSingle));
    if(NULL == pageProvider) {
        return NULL;
    }
    
    pageProvider->providerType = EaglePageProviderTypeSingle;
    pageProvider->name = (NULL == name ? NULL : strdup(name));
    pageProvider->type = EagleDataTypeInteger;
    pageProvider->recordsPerPage = recordsPerPage;
    pageProvider->value = value;
    
    return pageProvider;
}

void EaglePageProviderSingle_Delete(EaglePageProviderSingle *epp)
{
    if(NULL == epp) {
        return;
    }
    
    EagleMemory_Free(epp->name);
    EagleMemory_Free(epp);
}

EaglePage* EaglePageProviderSingle_nextPage(EaglePageProviderSingle *epp)
{
    EaglePage *page;
    int *data = (int*) EagleMemory_MultiAllocate("EaglePageProviderSingle_nextPage.1", sizeof(int), epp->recordsPerPage);
    if(NULL == data) {
        return NULL;
    }
    
    page = EaglePage_New(EagleDataTypeInteger, data, 1, 1, 0, EagleFalse);
    
    return page;
}

void EaglePageProviderSingle_reset(EaglePageProviderSingle *epp)
{
    /* do nothing */
}

int EaglePageProviderSingle_pagesRemaining(EaglePageProviderSingle *epp)
{
    /* unlimited supply of pages */
    return 1;
}
