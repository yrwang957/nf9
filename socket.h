#include <stdio.h>
#include <stdlib.h>

#define BINDING_ADDR "192.168.12.119"
#define BINDING_PORT 8000

//return value
#define SUCCESS  0
#define FAILED  -1

//TODO move to terminal function
pthread_t ts;

char sockBuf[65535];
int nbytes;

const int initSocket();
const int receive();
