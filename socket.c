#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "socket.h"
#include "def.h"
n
int _fd;
struct sockaddr_in _from;

const int init_socket(int bindPort)
{
    bytes = 0;
    memset(sock_buf, 0, sizeof(sock_buf));

    if((_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        printf("Error while socket\n");
        return FAILED;
    }

    _from.sin_family = AF_INET;
    _from.sin_addr.s_addr = htonl(INADDR_ANY);
    _from.sin_port = htons(bindPort);
    if(bind(_fd, (struct sockaddr*)&_from, sizeof(_from)) < 0)
    {
        printf("Error while binding \n");
        return FAILED;
    }

    printf("Socket created in :%d\n", bindPort);

    return SUCCESS;
}

const int receive()
{
    int _fromLen = sizeof(_from);

    bytes = recvfrom(_fd, sock_buf, sizeof(sock_buf), 0, (struct sockaddr*)&_from, (socklen_t *)&_fromLen);
    if(bytes <= 0)
    {
        printf("%s\n", bytes == 0 ? "received zero len" : "Error while receiving");
        return FAILED;
    }

    printf("----------------------------\n");
    printf("  UDP received %5d bytes\n", bytes);
    printf("----------------------------\n");

    // char* p = sock_buf;
    // unsigned char ff = 0xff;
    // printf("  %02x %02x %02x %02x\n",   p[ 0]&ff, p[ 1]&ff, p[ 2]&ff, p[ 3]&ff);
    // printf("  %02x %02x %02x %02x\n",   p[ 4]&ff, p[ 5]&ff, p[ 6]&ff, p[ 7]&ff);
    // printf("  %02x %02x %02x %02x\n",   p[ 8]&ff, p[ 9]&ff, p[10]&ff, p[11]&ff);
    // printf("  %02x %02x %02x %02x\n",   p[12]&ff, p[13]&ff, p[14]&ff, p[15]&ff);
    // printf("  %02x %02x %02x %02x\n\n", p[16]&ff, p[17]&ff, p[18]&ff, p[19]&ff);

    return SUCCESS;
}
