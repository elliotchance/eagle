#include <string.h>
#include "EaglePageProviderSingle.h"
#include "EagleMemory.h"
#include "EaglePageProviderArray.h"

EaglePageProviderSingle* EaglePageProviderSingle_NewInt(EagleDataTypeIntegerType value, int recordsPerPage, char *name)
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

EaglePageProviderSingle* EaglePageProviderSingle_NewFloat(EagleDataTypeFloatType value, int recordsPerPage, char *name)
{
    EaglePageProviderSingle *pageProvider = (EaglePageProviderSingle*) EagleMemory_Allocate("EaglePageProviderSingle_NewFloat.1", sizeof(EaglePageProviderSingle));
    if(NULL == pageProvider) {
        return NULL;
    }
    
    pageProvider->providerType = EaglePageProviderTypeSingle;
    pageProvider->name = (NULL == name ? NULL : strdup(name));
    pageProvider->type = EagleDataTypeFloat;
    pageProvider->recordsPerPage = recordsPerPage;
    pageProvider->value.floatValue = value;
    
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
        case EagleDataTypeFloat:
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
    
    if(NULL == epp) {
        return NULL;
    }
    
    /* fill page */
    switch(epp->type) {
            
        case EagleDataTypeUnknown:
            return NULL;
            
        case EagleDataTypeInteger:
        {
            EagleDataTypeIntegerType *data;
            
            /* allocate data for page */
            data = (EagleDataTypeIntegerType*) EagleMemory_MultiAllocate("EaglePageProviderSingle_nextPage.1", sizeof(EagleDataTypeIntegerType), epp->recordsPerPage);
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
            EagleDataTypeVarcharType *data;
            
            /* allocate data for page */
            data = (EagleDataTypeVarcharType*) EagleMemory_MultiAllocate("EaglePageProviderSingle_nextPage.2", sizeof(EagleDataTypeVarcharType), epp->recordsPerPage);
            if(NULL == data) {
                return NULL;
            }
            
            for(i = 0; i < epp->recordsPerPage; ++i) {
                data[i] = strdup(epp->value.strValue);
            }
            
            page = EaglePage_New(EagleDataTypeVarchar, data, epp->recordsPerPage, epp->recordsPerPage, 0, EagleTrue);
            break;
        }
            
        case EagleDataTypeFloat:
        {
            EagleDataTypeFloatType *data;
            
            /* allocate data for page */
            data = (EagleDataTypeFloatType*) EagleMemory_MultiAllocate("EaglePageProviderSingle_nextPage.3", sizeof(EagleDataTypeFloatType), epp->recordsPerPage);
            if(NULL == data) {
                return NULL;
            }
            
            for(i = 0; i < epp->recordsPerPage; ++i) {
                data[i] = epp->value.floatValue;
            }
            
            page = EaglePage_New(EagleDataTypeFloat, data, epp->recordsPerPage, epp->recordsPerPage, 0, EagleTrue);
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
