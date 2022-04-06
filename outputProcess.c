#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "outputProcess.h"

void outputProcess()
{
    for(;;)
    {
        printf("%d\n", 2);
        sleep(1);
    }
}
