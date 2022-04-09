#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "buffer.h"


void _eraseBuf(B* b);

int initBuf()
{
    int i;
    for(i = 0; i < BUF_SIZE; ++i)
    {
        _eraseBuf(&bs[i]);
    }

    return SUCCESS;
}

void _eraseBuf(B* b)
{
    b->using  = false;
    b->time   = 0;
    b->id     = -1;
    b->length = 0;
    b->type   = 0;
    b->p      = NULL;
}

int putBuf(int type, int length, int id, void* p)
{
    int ret = SUCCESS;
    int i = 0;
    int iU = 0; //idx of update
    int iN = 0; //idx of new

    for(iU = 0 ; iU < BUF_SIZE ; ++iU)
        if(bs[iU].id == id)
            break;
    //TODO: no need if iU targeted
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
        ret = FAILED;
    }

    return ret;
}
