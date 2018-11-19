#include <sys/types.h>
#include <sys/socket.h>                         // 包含套接字函数库
#include <stdio.h>
#include <netinet/in.h>                         // 包含AF_INET相关结构
#include <arpa/inet.h>                      // 包含AF_INET相关操作的函数
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <pthread.h>

#define MYPORT  6666
#define BUFFER_SIZE 1024

typedef struct
{
    int ab;
    int num[1000000];
}Node;

int main()
{
        ///sockfd
    int sock_cli = socket(AF_INET,SOCK_STREAM, 0);

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
       servaddr.sin_port = htons(MYPORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("connect");
        exit(1);
    }

    Node *myNode=(Node*)malloc(sizeof(Node));
    myNode->ab=123;
    myNode->num[0]=110;
    myNode->num[999999]=99;

    int needSend=sizeof(Node);
    char *buffer=(char*)malloc(needSend);
    memcpy(buffer,myNode,needSend);

    int pos=0;
    int len=0;
    while(pos < needSend)
    {
        len=send(sock_cli, buffer+pos, BUFFER_SIZE,0);
        if(len <= 0)
        {
            perror("ERRPR");
            break;
        }
        pos+=len;
    }
    free(buffer);
    free(myNode);
    close(sock_cli);
    printf("Send over!!!\n");
    return 0;
}