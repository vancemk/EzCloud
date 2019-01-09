/*************************************************************************
 * File Name: server.c
 * Author:    The answer
 * Function:  Other        
 * Mail:      2412799512@qq.com 
 * Created Time: 2017年06月15日 星期四 17时02分26秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/socket.h>
#include<signal.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>
#define SERV_PORT 8000

#include "databuffer.h"
#include "head.h"

struct Head;

#define READ_WRITE_SIZE 8192

//封装出错函数
void sys_err(const char *ptr,int num)
{
    perror(ptr);
    exit(num);
}

int main(int argc,char **argv)
{
    signal(SIGPIPE,SIG_IGN);
    int sockfd,accefd;
    struct sockaddr_in seraddr,cliaddr;
    socklen_t len;

    bzero(&seraddr,sizeof(seraddr));
    bzero(&cliaddr,sizeof(cliaddr));

    //socket
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd < 0)
    {
        sys_err("socket",-1);
    }

    //初始化ip地址+port端口号
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(SERV_PORT);
    seraddr.sin_addr.s_addr = htonl(INADDR_ANY);

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
	//accept
	
	//init databuffer
	DataBuffer dbuf;
	printf("db.getFreeLen(): %d\n", dbuf.getFreeLen());

	// init head
	struct Head testHead;

    while(1)
    {
        len = sizeof(cliaddr);
        accefd = accept(sockfd,(struct sockaddr *)&cliaddr,&len);
        if(accefd < 0)
        {
            if(errno == EINTR)  //判断阻塞等待客户端的链接;是被信号打断还是其它因素
                continue;
            else
                sys_err("accept",-4);
        }
        while(1)
        {
			dbuf.ensureFree(READ_WRITE_SIZE);
            //int leng = read(accefd, dbuf.getFree(), sizeof(testHead));
            int leng = read(accefd, dbuf.getFree(), dbuf.getFreeLen());
			printf("received: %d bytes date\n", leng);
			dbuf.pourData(leng);
            if(leng == 0)
            {
                printf("Opposite have close the socket.\n"); 
                break; //表示文件已经读到了结尾,也意味着客户端关闭了socket
            }
            if(leng == -1 && errno == EINTR)
                continue;
            if(leng == -1 )
                break; //表示出现了严重的错误
			//printf("read success: %g?\n", dbuf.readBytes((void *)&testHead, sizeof(testHead)));
			dbuf.readBytes((void *)&testHead, sizeof(testHead));
			printHead(&testHead);
        }

        //若文件的读写已经结束,则关闭文件描述符
        close(accefd);
    }
    close(sockfd);
	


    return 0;
}