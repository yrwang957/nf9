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

    //TODO: too complex
    if(type == BUF_DATA)
    {
        for(i = 0; i < BUF_SIZE; ++i)
            if( (bs[i].id == -1) && (bs[i].using == false) )
                break;
    }
    else
    {
        for(i = 0; i < BUF_SIZE; ++i)
            if((bs[i].id == id) && (bs[i].using == false) && bs[i].type == type)
                break;

        if(i == BUF_SIZE)
        {
            for(i = 0; i < BUF_SIZE; ++i)
                if( (bs[i].id == -1) && (bs[i].using == false) )
                    break;
        }
    }

    if(i != BUF_SIZE)
    {
        bs[i].using = true;
        bs[i].time = time(NULL);
        bs[i].id = id;
        bs[i].length = length;
        bs[i].type = type;

        if(bs[i].p != NULL)
        {
            free(bs[i].p);
            bs[i].p = NULL;
        }
        bs[i].p = (void*)malloc(length);
        memcpy(bs[i].p, p, length);

        bs[i].using = false;

        printf("      Use %03d buffer\n", i);
    }
    else
    {
        printf("      Buffer full\n");
        ret = FAILED;
    }

    return ret;
}
