#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>

#include "main.h"
#include "def.h"
#include "nf9headers.h"
#include "udp.h"

void _showPacketHeader(NetFlow9_header* packet_header);

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

        NetFlow9_header* packet_header = (NetFlow9_header*)sock_buf;
        uint16_t version = ntohs(packet_header->version);
        uint16_t count = ntohs(packet_header->count);

        _showPacketHeader(packet_header);

        if(version != 9)
        {
            printf("[WARN] Drop due to version %hu\n", version);
            continue;
        }

        // +----------------+
        // | Unpack FlowSet |
        // +----------------+
        uint16_t length = 0;
        int returned_count = 0;
        int processed_count = 0;
        int processed_byte = sizeof(NetFlow9_header);
        uint8_t* ptr_buf = sock_buf + sizeof(NetFlow9_header);

        // Foreach FlowSet
        while((bytes - processed_byte >= 4) && (count > processed_count))
        {
            FlowSet_header* fs_header = (FlowSet_header*)ptr_buf;
            length = ntohs(fs_header->length);

            printf("Segment %u:\n", processed_count + 1);

            returned_count = process_flowSet(fs_header);

            processed_count += returned_count; // Accumulate count(reference to v9 header)
            processed_byte += (int)length; // Accumulate processed byte(reference to udp received)
            ptr_buf += (uint8_t)length; // Pointer forward to next

            printf("  Processed count: %d/%d\n", processed_count, count);
        }
        printf("\n");

        fflush(stdout);
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

    // TODO: delete
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

void _showPacketHeader(NetFlow9_header* header)
{
    char time_str[256] = {};
    uint16_t version = ntohs(header->version);
    uint16_t count = ntohs(header->count);
    uint32_t sysUptime = ntohl(header->systemUpTime);
    uint32_t unixSec = ntohl(header->unixSeconds);
    uint32_t packetSeq = ntohl(header->packetSeq);
    uint32_t sourceId = ntohl(header->sourceId);

    time_t unixSec_timet = (time_t)unixSec;
    struct tm* unixSec_tm = localtime( &unixSec_timet );

    snprintf(time_str, sizeof(time_str), "%u day %u hr %u min %u sec",
        (sysUptime) / 86400000,
        (sysUptime) / 3600000 % 24,
        (sysUptime) / 60000 % 60,
        (sysUptime) / 1000 % 60);

    printf("+--------------------------------------------+\n");
    printf("| Header                                     |\n");
    printf("+--------------+-----------------------------+\n");
    printf("| version      | %-10hu                  |\n",  version);
    printf("| count        | %-10hu                  |\n",  count);
    printf("| systemUpTime | %-27s |\n", time_str);
    printf("| unixSeconds  | %4d-%02d-%02d %02d:%02d:%02d         |\n",
        unixSec_tm->tm_year + 1900,
        unixSec_tm->tm_mon + 1,
        unixSec_tm->tm_mday,
        unixSec_tm->tm_hour,
        unixSec_tm->tm_min,
        unixSec_tm->tm_sec);
    printf("| packetSeq    | %-10u                  |\n", packetSeq);
    printf("| sourceId     | %-10u                  |\n", sourceId);
    printf("+--------------+-----------------------------+\n\n");

    return;
}
