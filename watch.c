#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <time.h>
#include <string.h>

#include "watch.h"

//test
#include "buffer.h"

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
        tv.tv_sec = 10;
        tv.tv_usec = 100000; //1000000 = 1sec
        select(0, NULL, NULL, NULL, &tv);
        //printf("watch wake-up\n");

        //test
        printf("Buffer:\n");
        int i;
        for(i = 0; i < BUF_SIZE; ++i)
        {
            if(bs[i].id != -1)
            {
                printf("%03d, id %d, time %u, len %d, type %d, p %p\n", i, bs[i].id, (int)bs[i].time, bs[i].length, bs[i].type, bs[i].p );
            }
        }

    }
}
