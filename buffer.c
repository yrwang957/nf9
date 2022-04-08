#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "buffer.h"

void initBuffer()
{
    int i;
    for(i = 0 ; i < BUF_SIZE ; ++i)
    {
        bs[i].using = false;
        bs[i].time = 0;
        bs[i].id = -1;
        bs[i].length = 0;
        bs[i].type = 0;
        bs[i].p = NULL;
    }
}

int enB(int type, int length, int id, void* p)
{
    int ret = 0;
    int i = 0;
    int iU = 0; //idx of update
    int iN = 0; //idx of new

    for(iU = 0 ; iU < BUF_SIZE ; ++iU)
        if(bs[iU].id == id)
            break;

    for(iN = 0 ; iN < BUF_SIZE ; ++iN)
        if((bs[iN].id == -1) && (bs[iN].using == false))
            break;

    if((iU < BUF_SIZE) || (iN < BUF_SIZE))
    {
        i = (iU < BUF_SIZE)? iU : iN;

        bs[i].using = true;
        bs[i].time = time(NULL);
        bs[i].id = id;
        bs[i].length = length;
        bs[i].type = type;

        if(bs[i].p != NULL)
        {
            free(bs[i].p);
        }
        bs[i].p = (void*)malloc(length);
        memcpy(bs[i].p, p, length);

        bs[i].using = false;

        printf("Use no.%03d buffer\n", i);
    }
    else
    {
        printf("Buffer is full\n");
        ret = -1;
    }

    return ret;
}
