#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#define NEW_TEMPLATE 1000
#define UPDATED      1001
#define EXPIRED      1002

//flowset types in buffer
#define BUF_TMPLATE   2001
#define BUF_OTEMPLATE 2002
#define BUF_DATA      2003

//fixed buffer size
#define BUF_SIZE     256

//return value
#define SUCCESS      0
#define FAILED      -1
#define BUF_FULLED  -2

typedef struct _B
{
    bool   using;
    time_t time;
    int    id;
    int    length;
    int    type;
    void*  p;
}B;

B bs[BUF_SIZE];

void initBuffer();
int enB(int, int, int, void*);
