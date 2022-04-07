
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

#define BINDING_ADDR "192.168.12.119"
#define BINDING_PORT 17500

int fd;
int nbytes;
struct sockaddr_in from;
char buffer[65535];

int constructUdp();
int receiveUdp();
