// Client side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <sys/un.h>
#include <pthread.h>

#define SOCK_PATH "/tmp/udp_server"

void *clientThread(void *args)
{
    int serverSock,clientSock;
    int serverLen, clientLen;
    int rc, byterec;
    char readBuffer[200], writeBuffer[200];
    char *hello = "Hello from client"; 
    struct sockaddr_un serverSockAddr, clientSockAddr;

    //create socket file descriptor
    clientSock = socket(AF_UNIX, SOCK_DGRAM,0);


    //fill socket info
    clientSockAddr.sun_family = AF_UNIX;
    strcpy(clientSockAddr.sun_path,SOCK_PATH);
    clientLen = sizeof(clientSockAddr);
    serverLen = sizeof(serverSockAddr);

    //send data
    sendto(clientSock,(const char *)hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr*) &serverSockAddr, serverLen);
    printf("message sent to server\n");


    byterec = recvfrom(clientSock, readBuffer, serverLen, MSG_WAITALL, (struct sockaddr *) &serverSockAddr, &serverLen);
    readBuffer[byterec] = '\0';
    printf("Server :  %s \n", readBuffer);
    close(clientSock);
    return NULL;

}

int main()
{
    pthread_t clientT;
    pthread_create(&clientT,NULL,clientThread,NULL);
    pthread_join(clientT,NULL);
    printf("thread done\n");
    return 0;
}