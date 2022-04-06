#include <stdio.h>
#include <stdlib.h>

#define BUFFSIZE 4

typedef struct _Record
{
    char* record;
    int len;
}Record;

int enQueue(char*, int);
int deQueue(char*, int*);

void bufferManagement();
