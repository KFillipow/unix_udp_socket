// Server side implementation of UDP client-server model 
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

#define SERVER_PATH "/tmp/udp_server.server"
#define CLIENT_PATH "/tmp/udp_server.client"

void *serverThread(void *args)
{
    int serverSock,clientSock;
    int serverLen, clientLen;
    int rc, byterec;
    char readBuffer[200], writeBuffer[200];
    char *hello = "Hello from server"; 
    struct sockaddr_un serverSockAddr, clientSockAddr;

    //Create socket file descriptor for UNIX IPC
    serverSock = socket(AF_UNIX, SOCK_DGRAM,0);
    if(serverSock == 1)
    {
        printf("Error creating server socket\n");
        return NULL;
    }

    memset(&serverSockAddr,0,sizeof(serverSockAddr));
    memset(&clientSockAddr,0,sizeof(clientSockAddr));

    //Filling server info
    serverSockAddr.sun_family = AF_UNIX;
    strcpy(serverSockAddr.sun_path,SERVER_PATH);
    serverLen = sizeof(serverSockAddr);
    clientLen = sizeof(clientSockAddr);

    //unbind before binding again
    unlink(SERVER_PATH);
    rc = bind(serverSock, (struct sockaddr *) &serverSockAddr, serverLen);
    if(rc == -1)
    {
        //
        close(serverSock);
        return NULL;
    }

    bzero(readBuffer,sizeof(readBuffer));
    printf("waiting for data...\n");

    byterec = recvfrom(serverSock, readBuffer, sizeof(readBuffer), MSG_WAITALL, ( struct sockaddr * ) &clientSockAddr, &clientLen);
    if(byterec == -1)
    {
        printf("error receiving\n");
        return NULL;
    }
    readBuffer[byterec] = '\0';
    printf("Client :  %s \n", readBuffer);
    rc = sendto(serverSock, (const char *)hello, strlen(hello), 0, (const struct sockaddr *) &clientSockAddr, clientLen);
    if(rc == -1)
    {
        printf("error sending\n");
        return NULL;
    }
    printf("message sent\n");


    return NULL;
}


int main()
{
    pthread_t serverT;
    pthread_create(&serverT,NULL,serverThread,NULL);
    //sleep(5);
    pthread_join(serverT,NULL);
    printf("Thread done\n");
    return 0;
    //can receive data now

}