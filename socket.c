#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "socket.h"

int _fd;
struct sockaddr_in _from;

const int initSocket(int bindPort)
{
    memset(sockBuf, 0, sizeof(sockBuf));

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

const int receiveUdp()
{
    int nbytes;
    int _fromLen = sizeof(_from);

    nbytes = recvfrom(_fd, sockBuf, sizeof(sockBuf), 0, (struct sockaddr*)&_from, (socklen_t *)&_fromLen);
    if(nbytes <= 0)
    {
        printf("%s\n", nbytes == 0 ? "received zero len" : "Error while receiving");
        return FAILED;
    }

    printf("=UDP=\n");
    printf("received %d bytes:\n", nbytes);

    // char* p = sockBuf;
    // unsigned char ff = 0xff;
    // printf("  %02x %02x %02x %02x\n",       p[ 0]&ff, p[ 1]&ff, p[ 2]&ff, p[ 3]&ff);
    // printf("  %02x %02x %02x %02x\n",       p[ 4]&ff, p[ 5]&ff, p[ 6]&ff, p[ 7]&ff);
    // printf("  %02x %02x %02x %02x\n",       p[ 8]&ff, p[ 9]&ff, p[10]&ff, p[11]&ff);
    // printf("  %02x %02x %02x %02x\n",       p[12]&ff, p[13]&ff, p[14]&ff, p[15]&ff);
    // printf("  %02x %02x %02x %02x \n\n",    p[16]&ff, p[17]&ff, p[18]&ff, p[19]&ff);

    return SUCCESS;
}
