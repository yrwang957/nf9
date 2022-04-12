#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>

#include "main.h"
#include "nf9r.h"
#include "buffer.h"
#include "socket.h"
#include "watch.h"

int main(int argc, char** argv)
{
    printf("NF9 start\n");

    init(argc, argv);

    run();

    pthread_join(tw, NULL);
    pthread_join(tw, NULL);

    printf("NF9 end\n");
    return 0;
}

void run()
{
    //TODO:

    //TODO: move to socket run
    int i = 0;
    for(;;)
    {
        int version = 0;
        int count = 0;

        if(receive() != SUCCESS)
        {
            continue;
        }

        NF9Header* pH = (NF9Header*)sockBuf;
        version = ntohs(pH->version);
        count = ntohs(pH->count);
        printf("Header:\n");
        printf("version      %u\n",   version);
        printf("count        %u\n",   count);
        printf("systemUpTime %u\n",   ntohl(pH->systemUpTime));
        printf("unixSeconds  %u\n",   ntohl(pH->unixSeconds));
        printf("packetSeq    %u\n",   ntohl(pH->packetSeq));
        printf("SourceId     %u\n\n", ntohl(pH->SourceId));

        if(version != 9)
        {
            printf("Drop due to version\n\n");
            continue;
        }

        // +----------------+
        // | Unpack FlowSet |
        // +----------------+
        int flowSetId = 0;
        int length = 0;
        int pLength = 20;
        char* p = sockBuf + sizeof(NF9Header);
        for(i = 0; pLength < nbytes; ++i)
        {
            FlowSetHeader* pFS = (FlowSetHeader*)p;
            flowSetId = ntohs(pFS->flowSetId);
            length = ntohs(pFS->length);

            printf("  %03d FlowSet:\n", i);
            printf("  id %d, len %d\n\n", flowSetId, length);

            switch(flowSetId)
            {
            // +----------------------+
            // | Template sets        |
            // +----------------------+
            case TEMPLATE_FLOWSET:
                templateFlowSet(pFS);
                break;

            // +----------------------+
            // | Option Template sets |
            // +----------------------+
            case OPTION_TEMPLATE:
                optionTemplate(pFS);
                break;

            // +----------------------+
            // | Data sets            |
            // +----------------------+
            default:
                data(pFS);
                break;
            }

            pLength += length;
            p += length;
        }
        printf("\n");
    }
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