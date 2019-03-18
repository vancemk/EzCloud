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
#include<sys/wait.h>
#define SERV_PORT 8000

#include "../Heads/databuffer.h"
#include "../Heads/head.h"
#include "../Heads/sockser.h"
#include "../Heads/fileparser.h"
#include "../Heads/headhandle.h"

#include "../Heads/epollsocketevent.h"

#define READ_WRITE_SIZE 1024
#define DEPOTDIR "./Z_DEPOTDIR/"

struct Head;
int getLisSock();
void sys_err(const char *ptr,int num);
void readHead(struct Head & rhead, DataBuffer & pdbuf, const int pconfd);
void readFile(struct Head * phead, DataBuffer & pdbuf, const int pconfd);

void handler(int sig)  
{  
    pid_t   pid;  
    int     stat;  
    /*  wait(NULL); //只能等待第一个退出的子进程 */  
    /* 即使因为几个连接同时断开，信号因不能排队而父进程只收到一个信号 
	 *      * 直到已经waitpid到所有子进程，返回0，才退出循环 */  
    while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)    
	    printf("child %d terminated\n", pid);    
	    printf("catch %d signal\n", sig);    
    return;  
}

int main(void)
{
    signal(SIGCHLD, handler);
    int sockfd = getLisSock();
	DataBuffer abc;
	EPollSocketEvent epev = EPollSocketEvent(sockfd, abc);
	epev.addEvent(sockfd, 1, 0);	


	epev.eventLoop();

	 

    return 0;
}

