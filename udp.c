#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "udp.h"
#include "def.h"

uint8_t sock_buf[65535];
int bytes;

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

/*
    char* p = (char*)sock_buf;
    unsigned char ff = 0xff;
    printf("  %02x %02x %02x %02x\n",   p[ 0]&ff, p[ 1]&ff, p[ 2]&ff, p[ 3]&ff);
    printf("  %02x %02x %02x %02x\n",   p[ 4]&ff, p[ 5]&ff, p[ 6]&ff, p[ 7]&ff);
    printf("  %02x %02x %02x %02x\n",   p[ 8]&ff, p[ 9]&ff, p[10]&ff, p[11]&ff);
    printf("  %02x %02x %02x %02x\n",   p[12]&ff, p[13]&ff, p[14]&ff, p[15]&ff);
    printf("  %02x %02x %02x %02x\n\n", p[16]&ff, p[17]&ff, p[18]&ff, p[19]&ff);
*/

    printf("[INFO] Received %d from %s:%d\n", bytes, inet_ntoa(from.sin_addr), from.sin_port);
    return SUCCESS;
}
