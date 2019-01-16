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

    if(bind(sockfd,(struct sockaddr *)&seraddr,sizeof(seraddr)) < 0)
    {
        sys_err("bind",-2);
    }

    if(listen(sockfd,128) < 0 )
    {
        sys_err("listen",-3);
    }
	
	char buf[64];
	memset(buf, 0, 64);
    while(1)
    {
        len = sizeof(cliaddr);
        accefd = accept(sockfd,(struct sockaddr *)&cliaddr,&len);
        if(accefd < 0)
        {
            if(errno == EINTR)  
                continue;
            else
                sys_err("accept",-4);
        }
        while(1)
        {
            int leng = read(accefd, buf, 14);
			printf("%s", (char *)buf);
			memset(buf, 0, 20);
        }

        //若文件的读写已经结束,则关闭文件描述符
        close(accefd);
    }
    close(sockfd);
	


    return 0;
}
