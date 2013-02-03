#include <stdio.h>
#include <stdlib.h>

#include "EagleDbConsole.h"

/**
 Entry point.
 @param [in] argc Arg count.
 @param [in] argv Arguments.
 @return Exit status.
 */
int main(int argc, const char * argv[])
{
    EagleDbConsole *console = EagleDbConsole_New();
    EagleDbConsole_run(console);
    EagleDbConsole_Delete(console);
    
    return 0;
}
