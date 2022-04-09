#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <time.h>
#include <string.h>

#include "watch.h"

int initWatch()
{
    return SUCCESS;
}

void watch()
{
    struct timeval tv;

    for(;;)
    {
        //+--------+
        //| things |
        //+--------+
        printf("watch wake-up\n");

        tv.tv_sec = 5;
        tv.tv_usec = 100000; //1000000 = 1sec
        select(0, NULL, NULL, NULL, &tv);
    }
}
