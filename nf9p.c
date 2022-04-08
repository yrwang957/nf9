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
#include "buffer.h"

int main(int argc, char** argv)
{
    int i;
    int ret;
    int bindPort;

    printf("NFv9 parser Start\n");

    bindPort = (argc >= 2) ? atoi(argv[1]) : BINDING_PORT;
    if((ret = constructUdp(bindPort)) < 0)
    {
        printf("Error in constructUdp\n");
        return -1;
    }

    initBuffer();

    for(;;)
    {
        if((ret = receiveUdp()) < 0)
        {
            continue;
        }

        NF9Header* pH = (NF9Header*)buffer;
        printf("=Header=\n");
        printf("version      :%u\n",   (ntohs)(pH->version));
        printf("count        :%u\n",   (ntohs)(pH->count));
        printf("systemUpTime :%u\n",   (ntohl)(pH->systemUpTime));
        printf("unixSeconds  :%u\n",   (ntohl)(pH->unixSeconds));
        printf("packetSeq    :%u\n",   (ntohl)(pH->packetSeq));
        printf("SourceId     :%u\n\n", (ntohl)(pH->SourceId));

        FlowSetHeader* pFS = (FlowSetHeader*)(buffer + sizeof(NF9Header));
        printf("=FlowSet=\n");
        printf("FlowSet Id   :%u\n",    (ntohs)(pFS->flowSetId));
        printf("Length       :%u\n\n",  (ntohs)(pFS->length));

        if(((ntohs)(pH->version) != 9) || ((ntohs)(pH->count) >= 1024))
        {
            printf("Drop due to version or count\n\n");
            continue;
        }

        // +-----------------+
        // | Unpack FlowSet  |
        // +-----------------+
        printf("=Records=\n");
        for(i = 0 ; i < (ntohs)(pH->count) ; ++i)
        {
            int flowSetId = (ntohs)(pFS->flowSetId);
            int length = (ntohs)(pFS->length);

            printf("%03d : Id %u, Len %u\n", i, flowSetId, length);
            if(length <= sizeof(FlowSetHeader))
            {
//                printf("flowset length %d too less, continue\n", length);
                break;
            }

            switch(flowSetId)
            {
            // +-----------------+
            // | Template sets   |
            // +-----------------+
            case TEMPLATE_FLOWSET:
                templateFlowSet(pFS);
//                printf("templateFlowSet end\n\n");
            break;

            // +----------------------+
            // | Option Template sets |
            // +----------------------+
            case OPTION_TEMPLATE:
                optionTemplate(pFS);
            break;

            // +-----------------+
            // | Data sets       |
            // +-----------------+
            default:
                data(pFS);
                break;
            }

            pFS = (FlowSetHeader*)((char*)pFS + length);
        }
        printf("\n");

    }

    printf("NFv9 parser End\n");
    return 0;
}

int constructUdp(int bindPort)
{
    memset(buffer, '\0', sizeof(buffer));

    if((fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        printf("Error while socket\n");
        return -1;
    }

    from.sin_family = AF_INET;
    from.sin_addr.s_addr = htonl(INADDR_ANY);
    from.sin_port = htons(bindPort);
    if(bind(fd, (struct sockaddr*)&from, sizeof(from)) < 0)
    {
        printf("Error while binding \n");
        return -1;
    }

    printf("Socket created in :%d\n", bindPort);
    return 0;
}

int receiveUdp()
{
    unsigned char ff = 0xff;
    int fromLen = sizeof(from);

    nbytes = recvfrom(fd, buffer, sizeof(buffer), 0, (struct sockaddr*)&from, (socklen_t *)&fromLen);
    if(nbytes <= 0)
    {
        printf("%s\n", nbytes == 0 ? "received zero len" : "Error while receiving");
        return -1;
    }

    printf("=UDP=\n");
    printf("received %d bytes:\n", nbytes);

    char* p = buffer;
    printf("  %02x %02x %02x %02x\n",       p[ 0]&ff, p[ 1]&ff, p[ 2]&ff, p[ 3]&ff);
    printf("  %02x %02x %02x %02x\n",       p[ 4]&ff, p[ 5]&ff, p[ 6]&ff, p[ 7]&ff);
    printf("  %02x %02x %02x %02x\n",       p[ 8]&ff, p[ 9]&ff, p[10]&ff, p[11]&ff);
    printf("  %02x %02x %02x %02x\n",       p[12]&ff, p[13]&ff, p[14]&ff, p[15]&ff);
    printf("  %02x %02x %02x %02x \n\n",    p[16]&ff, p[17]&ff, p[18]&ff, p[19]&ff);

    return 0;
}

void initBuffer()
{
    int i;
    for(i = 0 ; i < BUF_SIZE ; ++i)
    {
        bs[i].using = false;
        bs[i].time = 0;
        bs[i].id = -1;
        bs[i].count = 0;
        bs[i].length = 0;
        bs[i].type = 0;
        bs[i].p = NULL;
    }
}
