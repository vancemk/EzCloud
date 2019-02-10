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
#include<sys/wait.h>
#define SERV_PORT 8000

#include "../Heads/databuffer.h"
#include "../Heads/head.h"
#include "../Heads/sockser.h"
#include "../Heads/fileparser.h"
#include "../Heads/headhandle.h"

#define READ_WRITE_SIZE 1024
 #define DEPOTDIR "./Z_DEPOTDIR/"

struct Head;
int getLisSock();
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
	int accefd;
    struct sockaddr_in cliaddr;
    socklen_t len;

    bzero(&cliaddr,sizeof(cliaddr));

	//init databuffer
	DataBuffer dbuf;
	dbuf.ensureFree(READ_WRITE_SIZE);
	printf("db.getFreeLen(): %d\n", dbuf.getFreeLen());

	// init head
	struct Head testHead;
	pid_t pid;
	
	char bufPwdPath[256];
	if (!getcwd(bufPwdPath, 256)) {
		printf("func main: get pwd path wrong, process terminated.\n");
		exit(-1);
	}
	printf("func main: PWD: %s\n\n", bufPwdPath);
	std::vector<std::string> vecPath;
	std::vector<struct Head> vecLocHeads;
	iterateDir(DEPOTDIR, vecPath);
	// getHeadInfo(vecHead, vecPath);
    getHeadInfo1(vecLocHeads, vecPath, bufPwdPath);
	std::vector<struct Head> vecRmtHeads;

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
		pid =  fork();
		if (pid == -1) {
			log_msg("fork error");
			exit(-1);
		}
		if (pid != 0) {
			log_msg("parent process jump out\n");
			close(accefd);
			continue;
		}

		close(sockfd);
		
		// start the first task
		log_msg("start the first task");
		readAll(dbuf, accefd);
        while(1)
        {
			readHead1(testHead, dbuf, accefd);
			if (testHead.isNextFile == -1) {
				log_msg("rcv task-finished flag");
				break;
			}

			if (1 == testHead.isNextFile) {
				readFile(&testHead, dbuf, accefd);
			}
				
			struct Head tmphead;
			vecRmtHeads.push_back(tmphead);
			copyHead(&vecRmtHeads[vecRmtHeads.size()-1], &testHead);
			// sleep(1);
			if (fcntl(accefd, F_GETFL, 0) < 0){
				log_msg("client has closed connection\n");
				break;
			}
        }
		
		std::vector<struct Head> vecDiffHeads;
		cmpVecInfos(vecLocHeads, vecRmtHeads, vecDiffHeads);
		log_msg("vecLocHeads size: %ld\nvecRmtHeads size: %ld\nvecDiffHeads \
				size: %ld\n", vecLocHeads.size(), vecRmtHeads.size(), vecDiffHeads.size());

		// start the second task
		dbuf.clear();
		for (auto &i:vecDiffHeads) {
			writeHead(&i, dbuf, accefd);
	        if (1 == i.isNextFile) {
	            writeFile(&i, dbuf, accefd);
		    }
		}
		vecDiffHeads[0].isNextFile = -1;
		writeHead(&vecDiffHeads[0], dbuf, accefd);
		writeAll(dbuf, accefd);

		log_msg("starts to sleep, ready for last transmission");
		dbuf.clear();
		readAll(dbuf, accefd);
		while (1) {
			readHead1(testHead, dbuf, accefd);

			if (1 == testHead.isNextFile) {
				readFile(&testHead, dbuf, accefd);
			}
			else if (-1 == testHead.isNextFile) {
				log_msg("rcv task-finished flag");	
				break;
			}
			
	        if (fcntl(accefd, F_GETFL, 0) < 0) {
	            log_msg("connected fd has closed");
	            break;
			}
		}
        //若文件的读写已经结束,则关闭文件描述符
        if (fcntl(accefd, F_GETFL, 0) < 0){ 
			log_msg("client has closed connection\n");
		    break;
		}
		else { 
			close(accefd);
		}
		break;

    }
    close(sockfd);
	

    return 0;
}
