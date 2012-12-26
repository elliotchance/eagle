#include <stdio.h>
#include <stdlib.h>

#include "EagleDbConsole.h"

int main(int argc, const char * argv[])
{
    EagleDbConsole *console = EagleDbConsole_New();
    EagleDbConsole_run(console);
    EagleDbConsole_Delete(console);
    
    return 0;
}
