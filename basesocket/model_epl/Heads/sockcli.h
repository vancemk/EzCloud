#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<errno.h>
#include<unistd.h>
#include "log.h"

#define MAXSIZE 4096
#define IP "127.0.0.1"
#define SERV_PORT 8000

#ifndef SOCKCLI_H_
#define SOCKCLI_H_

void sys_err(const char *ptr,int num)
{
    perror(ptr);
    exit(num);
}


int getConSock(){
	int sockfd;
    struct sockaddr_in addr;

    //建立socket套接字
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd < 0)
        sys_err("socket",-1);

    bzero(&addr,sizeof(addr));

    //初始化ip+port
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERV_PORT);
    addr.sin_addr.s_addr = inet_addr(IP);

    //connect将sockfd套接字描述符与服务器端的ip+port联系起来
    if(connect(sockfd,(struct sockaddr *)&addr,sizeof(addr)) < 0)
        sys_err("connect",-2);

	return sockfd;
}


#endif
