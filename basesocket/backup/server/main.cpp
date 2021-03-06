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
#include<string>
#define SERV_PORT 8000

#include "databuffer.h"
#include "head.h"
#include "sockser.h"
#include "fileparser.h"

struct Head;
int getLisSock();
void sys_err(const char *ptr,int num);

#define READ_WRITE_SIZE 8192

int main(int argc,char **argv)
{
    signal(SIGPIPE,SIG_IGN);
    int sockfd = getLisSock();
	int accefd;
    struct sockaddr_in cliaddr;
    socklen_t len;

    bzero(&cliaddr,sizeof(cliaddr));

	DataBuffer dbuf;
	printf("db.getFreeLen(): %d\n", dbuf.getFreeLen());



	// init head
	struct Head testHead;

	dbuf.ensureFree(READ_WRITE_SIZE);
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
            //int leng = read(accefd, dbuf.getFree(), sizeof(testHead));
        }

        //若文件的读写已经结束,则关闭文件描述符
        close(accefd);
    }
    close(sockfd);
	


    return 0;
}
