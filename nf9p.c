#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "nf9p.h"
#include "nf9r.h"

int main(int argc, char** argv)
{
    int ret;

    printf("NFv9 parser Start\n");

    if((ret = constructUdp()) < 0)
    {
        printf("Error in constructUdp\n");
        return -1;
    }

    for(;;)
    {
        if((ret=receiveUdp()) < 0)
        {
            continue;
        }

        NF9Header* pH = (NF9Header*)buffer;
        printf("=Header=\n");
        printf("version      :%u\n",    pH->version);
        printf("count        :%u\n",    pH->count);
        printf("systemUpTime :%u\n",    pH->systemUpTime);
        printf("unixSeconds  :%u\n",    pH->unixSeconds);
        printf("packetSeq    :%u\n",    pH->packetSeq);
        printf("SourceId     :%u\n\n",  pH->SourceId);

        FlowSetHeader* pFS = (FlowSetHeader*)(buffer + sizeof(NF9Header));
        printf("=FlowSet=\n");
        printf("FlowSet Id   :%u\n",    pFS->flowSetId);
        printf("Length       :%u\n\n",  pFS->length);

        switch(pFS->flowSetId)
        {
            //Template
            case FLOWSET_TEMPLATE:
            // +-----------------+
            // | template        |
            // +-----------------+

            break;
            //Option Template
            case FLOWSET_OTEMPLATE:
            // +-----------------+
            // | Option template |
            // +-----------------+

            break;
            //Data
            default:
            // +-----------------+
            // | data            |
            // +-----------------+

            break;
        }

    }

    printf("NFv9 parser End\n");
    return 0;
}

int constructUdp()
{
    memset(buffer, '\0', sizeof(buffer));

    if((fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        printf("Error while socket\n");
        return -1;
    }

    from.sin_family = AF_INET;
    from.sin_addr.s_addr = inet_addr(BINDING_ADDR);
    from.sin_port = htons(BINDING_PORT);
    if(bind(fd, (struct sockaddr*)&from, sizeof(from)) < 0)
    {
        printf("Error while binding \n");
        return -1;
    }

    printf("Socket created in %s:%d\n", BINDING_ADDR, BINDING_PORT);
    return 0;
}

int receiveUdp()
{
    int fromLen = sizeof(from);

    if((nbytes = recvfrom(fd, buffer, sizeof(buffer), 0, (struct sockaddr*)&from, (socklen_t *)&fromLen)) <= 0)
    {
        if(nbytes == 0)
        {
            printf("received zero len\n");
            return -1;
        }

        printf("Error while receiving\n");
        return -1;
    }

    printf("=UDP=\n");
    printf("received %d bytes:\n", nbytes);
    printf("  %02x %02x %02x %02x\n",       buffer[ 0], buffer[ 1], buffer[ 2], buffer[ 3]);
    printf("  %02x %02x %02x %02x\n",       buffer[ 4], buffer[ 5], buffer[ 6], buffer[ 7]);
    printf("  %02x %02x %02x %02x\n",       buffer[ 8], buffer[ 9], buffer[10], buffer[11]);
    printf("  %02x %02x %02x %02x\n",       buffer[12], buffer[13], buffer[14], buffer[15]);
    printf("  %02x %02x %02x %02x ...\n\n", buffer[16], buffer[17], buffer[18], buffer[19]);

    return 0;
}
