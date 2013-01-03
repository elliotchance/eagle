#ifndef eagle_EagleMemory_h
#define eagle_EagleMemory_h

void* EagleMemory_Allocate(char *id, size_t size);
void EagleMemory_Free(void *ptr);
void** EagleMemory_MultiAllocate(char *id, size_t size, int quantity);
void EagleMemory_MultiFree(void **ptr, int quantity);
void EagleMemory_MockReset(void);
void EagleMemory_Mock(char *id);

#endif
