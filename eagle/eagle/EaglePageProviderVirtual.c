#include <string.h>
#include "EaglePageProviderVirtual.h"
#include "EagleMemory.h"
#include "EagleLogger.h"

EaglePageProviderVirtual* EaglePageProviderVirtual_New(int recordsPerPage,
                                                       char *name,
                                                       EagleDataType type,
                                                       void *obj,
                                                       void (*free)(void*),
                                                       EagleBoolean (*add)(void*, void*),
                                                       int (*pagesRemaining)(void*),
                                                       EaglePage* (*nextPage)(void*),
                                                       void (*reset)(void*),
                                                       EaglePage* (*getPage)(void*, int))
{
    EaglePageProviderVirtual *pageProvider = (EaglePageProviderVirtual*) EagleMemory_Allocate("EaglePageProviderVirtual_New.1", sizeof(EaglePageProviderVirtual));
    if(NULL == pageProvider) {
        return NULL;
    }
    
    pageProvider->providerType = EaglePageProviderTypeVirtual;
    pageProvider->recordsPerPage = recordsPerPage;
    pageProvider->name = (NULL == name ? NULL : strdup(name));
    pageProvider->type = type;
    pageProvider->obj = obj;
    pageProvider->add = add;
    pageProvider->free = free;
    pageProvider->nextPage = nextPage;
    pageProvider->pagesRemaining = pagesRemaining;
    pageProvider->reset = reset;
    pageProvider->getPage = getPage;
    
    return pageProvider;
}

void EaglePageProviderVirtual_Delete(EaglePageProviderVirtual *epp)
{
    if(NULL == epp) {
        return;
    }
    
    if(NULL != epp->free) {
        epp->free(epp->obj);
    }
    EagleMemory_Free(epp->name);
    EagleMemory_Free(epp);
}

EagleBoolean EaglePageProviderVirtual_add(EaglePageProviderVirtual *epp, void *data)
{
    return epp->add(epp->obj, data);
}

int EaglePageProviderVirtual_pagesRemaining(EaglePageProviderVirtual *epp)
{
    return epp->pagesRemaining(epp->obj);
}

EaglePage* EaglePageProviderVirtual_nextPage(EaglePageProviderVirtual *epp)
{
    return epp->nextPage(epp->obj);
}

void EaglePageProviderVirtual_reset(EaglePageProviderVirtual *epp)
{
    epp->reset(epp->obj);
}

EagleBoolean EaglePageProviderVirtual_isRandomAccess(EaglePageProviderVirtual *epp)
{
    if(NULL == epp->getPage) {
        return EagleFalse;
    }
    return EagleTrue;
}

EaglePage* EaglePageProviderVirtual_getPage(EaglePageProviderVirtual *epp, int pageNumber)
{
    return epp->getPage(epp->obj, pageNumber);
}
