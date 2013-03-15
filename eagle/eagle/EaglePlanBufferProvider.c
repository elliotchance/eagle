#include <stdlib.h>
#include <stdio.h>
#include "EaglePlanBufferProvider.h"
#include "EagleMemory.h"
#include "EagleDbSqlValue.h"

EaglePlanBufferProvider* EaglePlanBufferProvider_NewWithProvider(int destinationBuffer,
                                                                 EaglePageProvider *provider,
                                                                 EagleBoolean freeProvider)
{
    EaglePlanBufferProvider *bp = (EaglePlanBufferProvider*) EagleMemory_Allocate("EaglePlanBufferProvider_NewWithProvider.1", sizeof(EaglePlanBufferProvider));
    if(NULL == bp) {
        return NULL;
    }
    
    bp->type = EaglePlanBufferProviderTypeProvider;
    bp->destinationBuffer = destinationBuffer;
    bp->value.provider.provider = provider;
    bp->value.provider.freeProvider = freeProvider;
    
    return bp;
}

EaglePlanBufferProvider* EaglePlanBufferProvider_NewWithValue(int destinationBuffer,
                                                              EagleDbSqlValue *value)
{
    EaglePlanBufferProvider *bp = (EaglePlanBufferProvider*) EagleMemory_Allocate("EaglePlanBufferProvider_NewWithValue.1", sizeof(EaglePlanBufferProvider));
    if(NULL == bp) {
        return NULL;
    }
    
    bp->type = EaglePlanBufferProviderTypeValue;
    bp->destinationBuffer = destinationBuffer;
    bp->value.value = value;
    
    return bp;
}

char* EaglePlanBufferProvider_toString(EaglePlanBufferProvider *bp)
{
    char *msg = (char*) EagleMemory_Allocate("EaglePlanBufferProvider_toString.1", 1024), *name;
    if(NULL == msg) {
        return NULL;
    }
    
    switch(bp->type) {
            
        case EaglePlanBufferProviderTypeProvider:
        {
            name = EagleDataType_typeToName(bp->value.provider.provider->type);
            sprintf(msg, "destination = %d, name = %s, type = %s", bp->destinationBuffer, bp->value.provider.provider->name, name);
            EagleMemory_Free(name);
            break;
        }
            
        case EaglePlanBufferProviderTypeValue:
        {
            name = EagleDbSqlValueType_toString(bp->value.value->type);
            sprintf(msg, "destination = %d, type = %s", bp->destinationBuffer, name);
            break;
        }
            
    }
    
    return msg;
}

void EaglePlanBufferProvider_Delete(EaglePlanBufferProvider *bp)
{
    if(NULL == bp) {
        return;
    }
    
    switch(bp->type) {
            
        case EaglePlanBufferProviderTypeProvider:
        {
            if(EagleTrue == bp->value.provider.freeProvider) {
                EaglePageProvider_Delete(bp->value.provider.provider);
            }
            break;
        }
            
        case EaglePlanBufferProviderTypeValue:
            break;
            
    }
    
    EagleMemory_Free(bp);
}
