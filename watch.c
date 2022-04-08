#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <time.h>
#include <string.h>

#include "watch.h"

void watch()
{
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 100000; //1000000 = 1sec

    for(;;)
    {
        //+--------+
        //| things |
        //+--------+

        select(0, NULL, NULL, NULL, &tv);
    }
}
