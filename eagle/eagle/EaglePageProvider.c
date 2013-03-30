#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "EaglePageProviderArray.h"
#include "EaglePageProviderSingle.h"
#include "EaglePageProviderStream.h"
#include "EaglePageProviderVirtual.h"

int EaglePageProvider_TotalPages(int totalRecords, int recordsPerPage)
{
    int pagesLeft;
    
    if(0 == totalRecords || 0 == recordsPerPage) {
        return 0;
    }
    
    pagesLeft = totalRecords / recordsPerPage;
    if(totalRecords % recordsPerPage > 0) {
        ++pagesLeft;
    }
    
    return pagesLeft;
}

void EaglePageProvider_Delete(EaglePageProvider *epp)
{
    if(NULL == epp) {
        return;
    }
    
    switch(epp->providerType) {
            
        case EaglePageProviderTypeArray:
            EaglePageProviderArray_Delete((EaglePageProviderArray*) epp);
            break;
            
        case EaglePageProviderTypeSingle:
            EaglePageProviderSingle_Delete((EaglePageProviderSingle*) epp);
            break;
            
        case EaglePageProviderTypeStream:
            EaglePageProviderStream_Delete((EaglePageProviderStream*) epp);
            break;
            
        case EaglePageProviderTypeVirtual:
            EaglePageProviderVirtual_Delete((EaglePageProviderVirtual*) epp);
            break;
            
    }
}

EagleBoolean EaglePageProvider_add(EaglePageProvider *epp, void *data)
{
    switch(epp->providerType) {
            
        case EaglePageProviderTypeArray:
        case EaglePageProviderTypeSingle:
            break;
            
        case EaglePageProviderTypeStream:
            return EaglePageProviderStream_add((EaglePageProviderStream*) epp, data);
            
        case EaglePageProviderTypeVirtual:
            return EaglePageProviderVirtual_add((EaglePageProviderVirtual*) epp, data);
            
    }
    
    return EagleFalse;
}

int EaglePageProvider_pagesRemaining(EaglePageProvider *epp)
{
    int r;
    
    switch(epp->providerType) {
            
        case EaglePageProviderTypeArray:
            r = EaglePageProviderArray_pagesRemaining((EaglePageProviderArray*) epp);
            break;
            
        case EaglePageProviderTypeSingle:
            r = EaglePageProviderSingle_pagesRemaining((EaglePageProviderSingle*) epp);
            break;
            
        case EaglePageProviderTypeStream:
            r = EaglePageProviderStream_pagesRemaining((EaglePageProviderStream*) epp);
            break;
            
        case EaglePageProviderTypeVirtual:
            r = EaglePageProviderVirtual_pagesRemaining((EaglePageProviderVirtual*) epp);
            break;
            
    }
    
    return r;
}

EaglePage* EaglePageProvider_nextPage(EaglePageProvider *epp)
{
    EaglePage *page;
    
    switch(epp->providerType) {
            
        case EaglePageProviderTypeArray:
            page = EaglePageProviderArray_nextPage((EaglePageProviderArray*) epp);
            break;
            
        case EaglePageProviderTypeSingle:
            page = EaglePageProviderSingle_nextPage((EaglePageProviderSingle*) epp);
            break;
            
        case EaglePageProviderTypeStream:
            page = EaglePageProviderStream_nextPage((EaglePageProviderStream*) epp);
            break;
            
        case EaglePageProviderTypeVirtual:
            page = EaglePageProviderVirtual_nextPage((EaglePageProviderVirtual*) epp);
            break;
            
    }
    
    return page;
}

void EaglePageProvider_reset(EaglePageProvider *epp)
{
    switch(epp->providerType) {
            
        case EaglePageProviderTypeArray:
            EaglePageProviderArray_reset((EaglePageProviderArray*) epp);
            break;
            
        case EaglePageProviderTypeSingle:
            EaglePageProviderSingle_reset((EaglePageProviderSingle*) epp);
            break;
            
        case EaglePageProviderTypeStream:
            EaglePageProviderStream_reset((EaglePageProviderStream*) epp);
            break;
            
        case EaglePageProviderTypeVirtual:
            EaglePageProviderVirtual_reset((EaglePageProviderVirtual*) epp);
            break;
            
    }
}

EagleBoolean EaglePageProvider_isRandomAccess(EaglePageProvider *epp)
{
    EagleBoolean r;
    
    switch(epp->providerType) {
            
        case EaglePageProviderTypeArray:
            r = EaglePageProviderArray_isRandomAccess((EaglePageProviderArray*) epp);
            break;
            
        case EaglePageProviderTypeSingle:
            r = EaglePageProviderSingle_isRandomAccess((EaglePageProviderSingle*) epp);
            break;
            
        case EaglePageProviderTypeStream:
            r = EaglePageProviderStream_isRandomAccess((EaglePageProviderStream*) epp);
            break;
            
        case EaglePageProviderTypeVirtual:
            r = EaglePageProviderVirtual_isRandomAccess((EaglePageProviderVirtual*) epp);
            break;
            
    }
    
    return r;
}

EaglePage* EaglePageProvider_getPage(EaglePageProvider *epp, int pageNumber)
{
    EaglePage *page;
    
    switch(epp->providerType) {
            
        case EaglePageProviderTypeArray:
            page = EaglePageProviderArray_getPage((EaglePageProviderArray*) epp, pageNumber);
            break;
            
        case EaglePageProviderTypeSingle:
            page = EaglePageProviderSingle_getPage((EaglePageProviderSingle*) epp, pageNumber);
            break;
            
        case EaglePageProviderTypeStream:
            page = EaglePageProviderStream_getPage((EaglePageProviderStream*) epp, pageNumber);
            break;
            
        case EaglePageProviderTypeVirtual:
            page = EaglePageProviderVirtual_getPage((EaglePageProviderVirtual*) epp, pageNumber);
            break;
            
    }
    
    return page;
}
