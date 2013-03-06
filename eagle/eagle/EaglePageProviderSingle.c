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
    pageProvider->value.intValue = value;
    
    return pageProvider;
}

EaglePageProviderSingle* EaglePageProviderSingle_NewVarchar(const char *value, int recordsPerPage, char *name)
{
    EaglePageProviderSingle *pageProvider = (EaglePageProviderSingle*) EagleMemory_Allocate("EaglePageProviderSingle_NewVarchar.1", sizeof(EaglePageProviderSingle));
    if(NULL == pageProvider) {
        return NULL;
    }
    
    pageProvider->providerType = EaglePageProviderTypeSingle;
    pageProvider->name = (NULL == name ? NULL : strdup(name));
    pageProvider->type = EagleDataTypeVarchar;
    pageProvider->recordsPerPage = recordsPerPage;
    pageProvider->value.strValue = (NULL == value ? NULL : strdup(value));
    
    return pageProvider;
}

void EaglePageProviderSingle_Delete(EaglePageProviderSingle *epp)
{
    if(NULL == epp) {
        return;
    }
    
    switch(epp->type) {
            
        case EagleDataTypeInteger:
        case EagleDataTypeUnknown:
            break;
            
        case EagleDataTypeVarchar:
            EagleMemory_Free((void*) epp->value.strValue);
            break;
            
    }
    
    EagleMemory_Free(epp->name);
    EagleMemory_Free(epp);
}

EaglePage* EaglePageProviderSingle_nextPage(EaglePageProviderSingle *epp)
{
    EaglePage *page;
    int i;
    
    /* fill page */
    switch(epp->type) {
            
        case EagleDataTypeUnknown:
            return NULL;
            
        case EagleDataTypeInteger:
        {
            int *data;
            
            /* allocate data for page */
            data = (int*) EagleMemory_MultiAllocate("EaglePageProviderSingle_nextPage.1", sizeof(int), epp->recordsPerPage);
            if(NULL == data) {
                return NULL;
            }
            
            for(i = 0; i < epp->recordsPerPage; ++i) {
                data[i] = epp->value.intValue;
            }
            
            page = EaglePage_New(EagleDataTypeInteger, data, epp->recordsPerPage, epp->recordsPerPage, 0, EagleTrue);
            break;
        }
            
        case EagleDataTypeVarchar:
        {
            char **data;
            
            /* allocate data for page */
            data = (char**) EagleMemory_MultiAllocate("EaglePageProviderSingle_nextPage.2", sizeof(char*), epp->recordsPerPage);
            if(NULL == data) {
                return NULL;
            }
            
            for(i = 0; i < epp->recordsPerPage; ++i) {
                data[i] = strdup(epp->value.strValue);
            }
            
            page = EaglePage_New(EagleDataTypeVarchar, data, epp->recordsPerPage, epp->recordsPerPage, 0, EagleTrue);
            break;
        }
            
    }
    
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
