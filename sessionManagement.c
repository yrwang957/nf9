#include <stdio.h>
#include <stdlib.h>

#include "sessionManagement.h"

void udpConnector()
{
    int fd;
    int nbytes;
    struct sockaddr_in from;
    int fromLen = sizeof(from);
    char buffer[4096];

    memset(buffer, '\0', sizeof(buffer));

    // Create socket
    if(fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        printf("Error while creating socket\n");
        exit(-1);
    }
    printf("Socket created successfully\n");

    // Set port and IP:
    from.sin_family = AF_INET;
    from.sin_port = htons(2000);
    from.sin_addr.s_addr = inet_addr("127.0.0.1");

    for(;;)
    {
        // Receive the server's response:
        if((nbytes = recvfrom(fd, buffer, sizeof(buffer), 0, (struct sockaddr*)&from, (socklen_t *)&fromLen)) <= 0)
        {
            if(nbytes == 0)
            {
                printf("Connection closed\n");
                close(fd);
                return 0;
            }

            printf("Error while receiving\n");
            return -1;
        }

        printf("Reveiced %d bytes:", nbytes );
        printf("%02x %02x %02x %02x", buffer[0], buffer[1], buffer[2], buffer[3] );
        printf("%02x %02x %02x %02x...\n", buffer[4], buffer[5], buffer[6], buffer[7] );
    }

    return 0;
}
