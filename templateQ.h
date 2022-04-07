#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

#define NEW_TEMPLATE 10000
#define UPDATED      10001

typedef struct _T
{
    bool     using;
    time_t   expTime;
    uint16_t tId;
    void*    pT;
}T;

T ts[256];
