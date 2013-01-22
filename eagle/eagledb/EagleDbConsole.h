#ifndef eagle_EagleDbConsole_h
#define eagle_EagleDbConsole_h

#include <mach/mach_time.h>

typedef struct {
    
    /**
     The absolute time when the console was started.
     */
    uint64_t startTime;
    
} EagleDbConsole;

EagleDbConsole* EagleDbConsole_New(void);

void EagleDbConsole_run(EagleDbConsole *console);

void EagleDbConsole_Delete(EagleDbConsole *console);

/**
 http://stackoverflow.com/a/314422/1470961
 */
char* EagleDbConsole_GetLine(void);

#endif
