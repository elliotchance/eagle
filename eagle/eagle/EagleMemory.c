#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "EagleMemory.h"

static char **EagleMemory_Mocks = NULL;
static int EagleMemory_MocksInUse = 0;

/**
 Allocate a single block of memory.
 
 Memory that is allocated will be zeroed out.
 
 @param [in] id This is a unique string used for mocking memory allocations. See EagleMemory_Mock()
 @param [in] size The amount of memory to allocate.
 @return The pointer to the new memory or NULL upon failure. Failures must be handled in your code and may need to
         cascade up to recover from the failure.
 */
void* EagleMemory_Allocate(char *id, size_t size)
{
    void *m = malloc(size);
    if(NULL != m) {
        return NULL;
    }
    memset(m, 0, (unsigned long) size);
    return m;
}

/**
 @brief Free a pointer.
 The pointer will be set to NULL. It is safe to call this function with NULL and hence to call it multiple times on the
 same pointer.
 
 @param [in] ptr Pointer to free.
 */
void EagleMemory_Free(void *ptr)
{
    free(ptr);
    ptr = NULL;
}

/**
 @brief Allocate a single block to support an array.
 
 There is still only a single allocation, this is eqivilent to calloc().
 
 @param [in] id This is a unique string used for mocking memory allocations. See EagleMemory_Mock()
 @param [in] size The size of a single array element.
 @param [in] quantity The amount of array items.
 */
void** EagleMemory_MultiAllocate(char *id, size_t size, int quantity)
{
    return (void**) EagleMemory_Allocate(id, (size_t) quantity * size);
}

/**
 Free an array of elements.
 
 @note This is not the eqivilent opposite of EagleMemory_MultiAllocate(). This function will free each array element,
       then the whole array itself.
 
 @param [in] ptr Pointer to array.
 @param [in] quantity The amount of items in the array.
 */
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

/**
 Reset the mocking session.
 
 Run this method at the top of each test case, before invoking EagleMemory_Mock()
 */
void EagleMemory_MockReset(void)
{
    EagleMemory_MocksInUse = 0;
}

/**
 Mocking memory allocations.
 
 This is used by the MemorySuite to mock malloc() returning NULL. Every EagleMemory_Allocate() and
 EagleMemory_MultiAllocate() requires a unique ID. This ID can be sent to EagleMemory_Mock() to make sure that
 allocation returns NULL and the program handles the failure correctly.
 
 You may have upto 32 mocks for a single test case.
 
 @param [in] id The ID of the allocation handle to mock.
 */
void EagleMemory_Mock(char *id)
{
    if(NULL == EagleMemory_Mocks) {
        EagleMemory_Mocks = (char**) calloc((size_t) 32, sizeof(char*));
        EagleMemory_MocksInUse = 0;
    }
    EagleMemory_Mocks[EagleMemory_MocksInUse++] = id;
}
