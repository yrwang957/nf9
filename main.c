#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>

#include "main.h"
#include "def.h"
#include "nf9r.h"
#include "buffer.h"
#include "udp.h"
#include "watch.h"

int main(int argc, char** argv)
{
    printf("NF9 start\n");

    init(argc, argv);

    run();

    printf("NF9 end\n");
    return 0;
}

void run()
{
    int i = 0;
    uint16_t version = 0;
    uint16_t count = 0;

    for(;;)
    {
        if(receive() != SUCCESS)
        {
            continue;
        }

        NF9Header* pH = (NF9Header*)sock_buf;
        version = ntohs(pH->version);
        count = ntohs(pH->count);
        printf("Header:\n");
        printf("version      %hu\n",  version);
        printf("count        %hu\n",  count);
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
        uint8_t* p = sock_buf + sizeof(NF9Header);
        for(i = 0; pLength < bytes; ++i)
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

    int bindPort = (argc >= 2) ? atoi(argv[1]) : DEFAULT_BINDING_PORT;
    if((ret = init_socket(bindPort)) != SUCCESS)
    {
        printf("[ ERR]init_socket() failed\n");
        return ret;
    }
/*
    if((ret = initBuf()) != SUCCESS)
    {
        printf("initsock_buf failed\n");
        return ret;
    }


    if((ret = initWatch()) != SUCCESS)
    {
        printf("initWatch failed\n");
        return ret;
    }
    */

    return ret;
}
