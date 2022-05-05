#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>

#include "main.h"
#include "def.h"
#include "nf9r.h"
#include "udp.h"

void showPacketHeader(NF9Header* pH);

int main(int argc, char** argv)
{
    printf("[INFO] NF9 start\n");

    init(argc, argv);

    run();

    printf("[INFO] NF9 end\n");

    return 0;
}

void run()
{
    while(true)
    {
        // +---------------------+
        // | Receive from socket |
        // +---------------------+
        if(receive() != SUCCESS)
        {
            continue;
        }

        NF9Header* pH = (NF9Header*)sock_buf;
        uint16_t version = ntohs(pH->version);
        uint16_t count = ntohs(pH->count);

        showPacketHeader(pH);

        if(version != 9)
        {
            printf("[WARN] Drop due to version %hu\n", version);
            continue;
        }

        // +----------------+
        // | Unpack FlowSet |
        // +----------------+
        uint16_t flowSetId = 0;
        uint16_t length = 0;
        int returned_count = 0;
        int processed_count = 0;
        int processed_byte = sizeof(NF9Header);
        uint8_t* ptr_buf = sock_buf + sizeof(NF9Header);

        // foreach FlowSet
        while((bytes - processed_byte >= 4) && (count > processed_count))
        {
            FlowSetHeader* pHeader = (FlowSetHeader*)ptr_buf;
            flowSetId = ntohs(pHeader->flowSetId);
            length = ntohs(pHeader->length);

            printf("  id %hu, len %hu\n\n", flowSetId, length);

            switch(flowSetId)
            {
            // Template sets
            case TEMPLATE_FLOWSET:
                returned_count = templateFlowSet(pHeader);
                break;

            // Option Template sets
            case OPTION_TEMPLATE:
               // ignore for now
                // returned_count = optionTemplate(pHeader);
                break;

            // Data sets
            default:
                returned_count = data(pHeader);
                break;
            }

            processed_count += returned_count; // accumulate count(reference to v9 header)
            processed_byte += (int)length; // accumulate processed byte(reference to udp received)
            ptr_buf += (uint8_t)length; // forward pointer to next FlowSet
            printf("processed count:%d/%d\n", processed_count, count);
        }
        printf("\n");
    } // while loop
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

void showPacketHeader(NF9Header* pH)
{
    uint16_t version = ntohs(pH->version);
    uint16_t count = ntohs(pH->count);
    uint32_t sysUptime = ntohl(pH->systemUpTime);
    uint32_t unixSec = (time_t)ntohl(pH->unixSeconds);
    uint32_t packetSeq = ntohl(pH->packetSeq);
    uint32_t sourceId = ntohl(pH->sourceId);

    time_t unixSec_timet = (time_t)unixSec;
    struct tm* unixSec_tm = localtime( &unixSec_timet );

    printf("+--------------+\n");
    printf("| Header       |\n");
    printf("+--------------+\n");
    printf("| version      | %hu\n",  version);
    printf("| count        | %hu\n",  count);
    printf("| systemUpTime | %u\n",   sysUptime);
    printf("| unixSeconds  | %hu (%4d-%02d-%02d %02d:%02d:%02d)\n",
        unixSec,
        unixSec_tm->tm_year + 1900,
        unixSec_tm->tm_mon + 1,
        unixSec_tm->tm_mday,
        unixSec_tm->tm_hour,
        unixSec_tm->tm_min,
        unixSec_tm->tm_sec);
    printf("| packetSeq    | %u\n",   packetSeq);
    printf("| SourceId     | %u\n", sourceId);
    printf("+--------------+\n\n");
}
