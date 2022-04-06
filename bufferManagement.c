#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "bufferManagement.h"

Record** records;
int ptrEn;
int ptrDe;

int enQueue(char* buf, int len)
{
    if((ptrDe == ptrEn + 1) || ((ptrDe == 0) && (ptrEn == BUFFSIZE - 1)))
    {
        return -1;
    }

    memcpy(records[ptrEn]->record, buf, len);
    records[ptrEn]->len = len;

    ++ptrEn;
    if(ptrEn >= BUFFSIZE)
    {
        ptrEn = 0;
    }

    //printf("enQueue\n");

    return 0;
}

int deQueue(char* buf, int* len)
{
    if(ptrDe >= ptrEn)
    {
        return -1;
    }

    memcpy(buf, records[ptrDe]->record, records[ptrDe]->len);
    *len = records[ptrDe]->len;

    ++ptrDe;
    if(ptrDe >= BUFFSIZE)
    {
        ptrDe = 0;
    }

    //printf("deQueue\n");

    return 0;
}

void bufferManagement()
{
    int i;

    records = (Record**)malloc(sizeof(Record*) * BUFFSIZE);
    for(i = 0; i < BUFFSIZE; ++i)
    {
        records[i] = (Record*)malloc(sizeof(Record));
    }
    ptrEn = 1;
    ptrDe = 0;

    for(;;)
    {
        sleep(1);
    }

    for(i = 0; i < BUFFSIZE; ++i)
    {
        free(records[i]);
    }
    free(records);
}
