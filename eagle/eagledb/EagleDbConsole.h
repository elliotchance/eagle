#ifndef eagle_EagleDbConsole_h
#define eagle_EagleDbConsole_h

#include <mach/mach_time.h>
#include "Eagle.h"

typedef struct {
    
    /**
     The absolute time when the console was started.
     */
    EAGLE_ATTR_NA uint64_t startTime;
    
} EagleDbConsole;

EagleDbConsole* EagleDbConsole_New(void);

void EagleDbConsole_run(EagleDbConsole *console);

/**
 Free the console.
 */
void EagleDbConsole_Delete(EagleDbConsole *console);

/**
 http://stackoverflow.com/a/314422/1470961
 */
char* EagleDbConsole_GetLine(void);

#endif
