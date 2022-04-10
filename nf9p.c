#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>

#include "nf9p.h"
#include "nf9r.h"
#include "buffer.h"
#include "socket.h"
#include "watch.h"

int main(int argc, char** argv)
{
    int i;
    pthread_t tw;

    printf("NFv9 parser start\n");

    init(argc, argv);

    pthread_create(&tw, NULL, (void*)watch, NULL);

    for(;;)
    {
        if(receive() != SUCCESS)
        {
            continue;
        }

        NF9Header* pH = (NF9Header*)sockBuf;
        printf("Header:\n");
        printf("version      %u\n",   ntohs(pH->version));
        printf("count        %u\n",   ntohs(pH->count));
        printf("systemUpTime %u\n",   ntohl(pH->systemUpTime));
        printf("unixSeconds  %u\n",   ntohl(pH->unixSeconds));
        printf("packetSeq    %u\n",   ntohl(pH->packetSeq));
        printf("SourceId     %u\n\n", ntohl(pH->SourceId));

        if(((ntohs(pH->version)) != 9) || ((ntohs(pH->count)) >= 1024))
        {
            printf("Drop due to version or count\n\n");
            continue;
        }

        // +-----------------+
        // | Unpack FlowSet  |
        // +-----------------+
        int flowSetId = 0;
        int length = 0;
        int accLength = sizeof(NF9Header);
        FlowSetHeader* pFS = (FlowSetHeader*)(sockBuf + sizeof(NF9Header));
        for(i = 0 ; i < ntohs(pH->count) ; ++i)
        {
            if(accLength >= nbytes)
            {
                break;
            }

            flowSetId = ntohs(pFS->flowSetId);
            length = ntohs(pFS->length);

            printf("  %03d FlowSet:\n", i);
            printf("  id %d, len %d\n\n", flowSetId, length);

            switch(flowSetId)
            {
            // +-----------------+
            // | Template sets   |
            // +-----------------+
            case TEMPLATE_FLOWSET:
                templateFlowSet(pFS);
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
            accLength += length;
        }
        printf("\n");
    }

    pthread_join(tw, NULL);

    printf("NFv9 parser end\n");
    return 0;
}



int init(int argc, char** argv)
{
    int ret = SUCCESS;
    int bindPort = (argc >= 2) ? atoi(argv[1]) : BINDING_PORT;

    if((ret = initBuf()) != SUCCESS)
    {
        printf("initsockBuf failed\n");
        return ret;
    }

    if((ret = initSocket(bindPort)) != SUCCESS)
    {
        printf("initSocket failed\n");
        return ret;
    }

    if((ret = initWatch()) != SUCCESS)
    {
        printf("initWatch failed\n");
        return ret;
    }

    return ret;
}