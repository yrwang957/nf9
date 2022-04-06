#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "flowSetManagement.h"
#include "bufferManagement.h"

void flowSetProcess()
{

    char* p = NULL;
    int l = 0;
    for(;;)
    {
        deQueue(p, &l);
    }
}