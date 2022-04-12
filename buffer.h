#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#define NEW_TEMPLATE 0
#define UPDATED      1
#define EXPIRED      2

//flowset types in buffer
#define BUF_TEMPLATE   1
#define BUF_OTEMPLATE 2
#define BUF_DATA      4

//fixed buffer size
#define BUF_SIZE     256

//return value
#define SUCCESS      0
#define FAILED      -1

typedef struct _B
{
    bool   using;
    time_t time;
    int    id;
    int    length;
    int    type;
    char*  p;
}B;

B bs[BUF_SIZE];

int initBuf();
void eraseBuf(B*);
int putBuf(int, int, int, void*);
