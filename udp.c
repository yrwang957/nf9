#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "udp.h"
#include "def.h"

uint8_t sock_buf[65535];
int bytes;

struct in_addr sender_address;
unsigned short sender_port;
static int fd;
static struct sockaddr_in from;

int init_socket(int bind_port)
{
    bytes = 0;
    memset(sock_buf, 0, sizeof(sock_buf));

    if((fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        printf("[ ERR] Error while socket\n");
        return FAILED;
    }

    from.sin_family = AF_INET;
    from.sin_addr.s_addr = htonl(INADDR_ANY);
    from.sin_port = htons(bind_port);
    if(bind(fd, (struct sockaddr*)&from, sizeof(from)) < 0)
    {
        printf("[ ERR] Error while binding \n");
        return FAILED;
    }

    printf("[INFO] Socket created in :%d\n", bind_port);
    return SUCCESS;
}

int receive()
{
    int fromLen = sizeof(from);

    bytes = recvfrom(fd, sock_buf, sizeof(sock_buf), 0, (struct sockaddr*)&from, (socklen_t *)&fromLen);
    if(bytes <= 0)
    {
        printf("[WARN] %s\n", bytes == 0 ? "Received zero len" : "Error while receiving");
        return FAILED;
    }
    sender_address = from.sin_addr;
    sender_port = from.sin_port;

    printf("[INFO] Received %d from %s:%d\n", bytes, inet_ntoa(from.sin_addr), from.sin_port);
    return SUCCESS;
}
