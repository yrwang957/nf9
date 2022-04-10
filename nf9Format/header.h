#include <stdio.h>
#include <stdlib.h>

typedef struct _NF9Header
{
    u_int16_t version;
    u_int16_t count;
    u_int32_t systemUpTime;
    u_int32_t unixSeconds;
    u_int32_t packetSeq;
    u_int32_t SourceId;
}NF9Header;
