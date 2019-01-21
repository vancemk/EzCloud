/*************************************************************************
    * File Name: client.c
    * Author:    The answer
    * Function:  Other        
    * Mail:      2412799512@qq.com 
    * Created Time: 2017年06月15日 星期四 18时54分22秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<errno.h>
#include<unistd.h>
#define MAXSIZE 4096
#define IP "127.0.0.1"
#define SERV_PORT 8000

#include "databuffer.h"

void sys_err(const char *ptr,int num)
{
    perror(ptr);
    exit(num);
}

void writeAll(DataBuffer & pdbuf, const int pconfd) ;
int main(int argc,char **argv)
 {
    // ./a.out src
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




	DataBuffer dbuf, dbuf1;
	char buf[2048];
	dbuf.ensureFree(2048);
	printf("getfreelen: %d\n", dbuf.getFreeLen());
	for(int i=0; i<2018; i++)
		buf[i] = 'h';
	dbuf.writeBytes((void *)buf, 2048);
	dbuf1 = dbuf;
	printf("dbuf.getDataLen: %d\n", dbuf.getDataLen());
	printf("dbuf.getFreeLen: %d\n", dbuf.getFreeLen());
	
    close(sockfd);
    return 0;
 }


