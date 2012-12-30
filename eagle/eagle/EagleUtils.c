#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include "EagleUtils.h"

void EagleUtils_Fatal(char *msg)
{
    printf("\nFATAL: %s\n", msg);
    raise(SIGABRT);
    exit(1);
}
