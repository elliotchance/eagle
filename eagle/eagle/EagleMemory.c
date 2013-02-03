#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "EagleMemory.h"

/**
 The current mocks.
 */
static char **EagleMemory_Mocks = NULL;

/**
 The number of mocks in use.
 */
static int EagleMemory_MocksInUse = 0;

/**
 The number of times a mocked malloc() has been invoked.
 */
static int EagleMemory_MockInvocations = 0;

void* EagleMemory_Allocate(char *id, size_t size)
{
    void *m;
    
#ifdef CUNIT
    /* if we are unit testing, check for mocking */
    int i;
    for(i = 0; i < EagleMemory_MocksInUse; ++i) {
        if(0 == strcmp(EagleMemory_Mocks[i], id)) {
            ++EagleMemory_MockInvocations;
            return NULL;
        }
    }
#endif
    
    /* try the real allocation */
    m = malloc(size);
    if(NULL == m) {
        return NULL;
    }
    
    /* zero out memory */
    memset(m, 0, (unsigned long) size);
    return m;
}

void EagleMemory_Free(void *ptr)
{
    free(ptr);
    ptr = NULL;
}

void** EagleMemory_MultiAllocate(char *id, size_t size, int quantity)
{
    return (void**) EagleMemory_Allocate(id, (size_t) quantity * size);
}

void EagleMemory_MultiFree(void **ptr, int quantity)
{
    int i;
    for(i = 0; i < quantity; ++i) {
        free(ptr[i]);
        ptr[i] = NULL;
    }
    free(ptr);
    ptr = NULL;
}

void EagleMemory_MockInit(void)
{
    EagleMemory_MocksInUse = 0;
    EagleMemory_MockInvocations = 0;
}

void EagleMemory_MockFinish(void)
{
    EagleMemory_MocksInUse = 0;
}

void EagleMemory_Mock(char *id)
{
    if(NULL == EagleMemory_Mocks) {
        EagleMemory_Mocks = (char**) calloc((size_t) 32, sizeof(char*));
        EagleMemory_MocksInUse = 0;
    }
    EagleMemory_Mocks[EagleMemory_MocksInUse++] = id;
}

int EagleMemory_GetMockInvocations(void)
{
    return EagleMemory_MockInvocations;
}
