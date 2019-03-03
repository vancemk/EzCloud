/*************************************************************************
 * File Name: server.c
 * Author:    The answer
 * Function:  Other        
 * Mail:      2412799512@qq.com 
 * Created Time: 2017年06月15日 星期四 17时02分26秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include "log.h"

#define SERV_PORT 8000

#ifndef SOCKSER_H_
#define SOCKSER_H_

//封装出错函数
void sys_err(const char *ptr,int num)
{
    perror(ptr);
    exit(num);
}

int getLisSock(){
    int sockfd;
    struct sockaddr_in seraddr;

    bzero(&seraddr,sizeof(seraddr));

    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd < 0)
    {
        sys_err("socket",-1);
    }

    //初始化ip地址+port端口号
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(SERV_PORT);
    seraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int on = 1;
   	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
		log_msg("socket error");

	//
    //bind
    if(bind(sockfd,(struct sockaddr *)&seraddr,sizeof(seraddr)) < 0)
    {
        sys_err("bind",-2);
    }

    //listen
    if(listen(sockfd,128) < 0 )
    {
        sys_err("listen",-3);
    }
	
	return sockfd;

}

#endif


