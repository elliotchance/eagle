#ifndef eagle_EagleMemory_h
#define eagle_EagleMemory_h

/**
 Allocate a single block of memory.
 
 Memory that is allocated will be zeroed out.
 
 @param [in] id This is a unique string used for mocking memory allocations. See EagleMemory_Mock()
 @param [in] size The amount of memory to allocate.
 @return The pointer to the new memory or NULL upon failure. Failures must be handled in your code and may need to
 cascade up to recover from the failure.
 */
void* EagleMemory_Allocate(char *id, size_t size);

/**
 @brief Free a pointer.
 The pointer will be set to NULL. It is safe to call this function with NULL and hence to call it multiple times on the
 same pointer.
 
 @param [in] ptr Pointer to free.
 */
void EagleMemory_Free(void *ptr);

/**
 @brief Allocate a single block to support an array.
 
 There is still only a single allocation, this is eqivilent to calloc().
 
 @param [in] id This is a unique string used for mocking memory allocations. See EagleMemory_Mock()
 @param [in] size The size of a single array element.
 @param [in] quantity The amount of array items.
 */
void** EagleMemory_MultiAllocate(char *id, size_t size, int quantity);

/**
 Free an array of elements.
 
 @note This is not the eqivilent opposite of EagleMemory_MultiAllocate(). This function will free each array element,
 then the whole array itself.
 
 @param [in] ptr Pointer to array.
 @param [in] quantity The amount of items in the array.
 */
void EagleMemory_MultiFree(void **ptr, int quantity);

/**
 Start the mocking session.
 
 Run this method at the top of each test case, before invoking EagleMemory_Mock()
 */
void EagleMemory_MockInit(void);

/**
 Mocking memory allocations.
 
 This is used by the MemorySuite to mock malloc() returning NULL. Every EagleMemory_Allocate() and
 EagleMemory_MultiAllocate() requires a unique ID. This ID can be sent to EagleMemory_Mock() to make sure that
 allocation returns NULL and the program handles the failure correctly.
 
 You may have upto 32 mocks for a single test case.
 
 @param [in] id The ID of the allocation handle to mock.
 */
void EagleMemory_Mock(char *id);

/**
 Finish the mocking session.
 
 Run this method at the end of each test case, after invoking EagleMemory_Mock()
 */
void EagleMemory_MockFinish(void);

/**
 * Get the number of times a mocked malloc has returned NULL since EagleMemory_MockInit().
 * @return The number of mock invocations.
 */
int EagleMemory_GetMockInvocations(void);

#endif
