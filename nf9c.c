#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

#include "session.h"
#include "bufferManagement.h"
#include "flowSetManagement.h"
#include "outputProcess.h"

int main(int argc, char** argv)
{
    printf("NetFlow v9 Collector Start\n");

    pthread_t tBufMgmt;
    pthread_t tFSMgmt;
    pthread_t tOutProc;

    pthread_create(&tBufMgmt, NULL, (void*)bufferManagement, NULL);
    pthread_create(&tFSMgmt, NULL, (void*)flowSetProcess, NULL);
    pthread_create(&tOutProc, NULL, (void*)outputProcess, NULL);

    udpConstruct();

    pthread_join(tBufMgmt, NULL);
    pthread_join(tFSMgmt, NULL);
    pthread_join(tOutProc, NULL);

    printf("NetFlow v9 Collector End\n");

    return 0;
}

