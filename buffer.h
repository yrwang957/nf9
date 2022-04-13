
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

//flowset types in buffer
#define BUF_TEMPLATE   1
#define BUF_OTEMPLATE 2
#define BUF_DATA      4

//fixed buffer size
#define BUF_SIZE     256

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
