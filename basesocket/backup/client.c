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

void sys_err(const char *ptr,int num)
{
    perror(ptr);
    exit(num);
}

int main(int argc,char **argv)
 {
    // ./a.out src
    int sockfd;
    char buf[MAXSIZE];
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


    //打开读的文件
    const char *src = argv[1];
    int fd = open(src,O_RDONLY);
    if(fd < 0)
        sys_err("open",-3);

    while(1)
    {
        int len = read(fd,buf,sizeof(buf));
        if(len == 0)
            break;

        int _tmp = 0;
        //考虑若socket缓冲区小于len的情况，见原理图1
        while(1)
        {
            int ret = write(sockfd,buf + _tmp, len - _tmp);
            if(ret > 0 )
                _tmp += ret;
            if(_tmp == ret)
                break;
            if(ret < 0)
            {
                perror("write");
                break;
            }
        }

    }

    close(sockfd);
    return 0;
 }


