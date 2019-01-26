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

#include <string>
#include <vector>

#include "../Heads/databuffer.h"
#include "../Heads/head.h"
#include "../Heads/headhandle.h"
#include "../Heads/fileparser.h"
#include "../Heads/sockcli.h"


/*
void sys_err(const char *ptr,int num)
{
    perror(ptr);
    exit(num);
} */
#define READ_WRITE_SIZE 1024
using namespace std;

void writeAll(DataBuffer & pdbuf, const int pconfd) ;
int main(int argc,char **argv)
 {

	int sockfd = getConSock();

	DataBuffer dbuf, dbuf1;
	dbuf.ensureFree(READ_WRITE_SIZE);
	printf("dbuf.getDataLen: %d\n", dbuf.getDataLen());
	printf("dbuf.getFreeLen: %d\n", dbuf.getFreeLen());
	
	vector<std::string> vecPath;
	vector<struct Head> vecHead;
	iterateDir(argv[1], vecPath);
	getHeadInfo(vecHead, vecPath);
	
	for (auto i=0; i<3000; i++){
		vecHead[0].isNextFile = i;
		printHead(&vecHead[0]);
		writeHead(&vecHead[0], dbuf, sockfd);
		sleep(2);
	}

    close(sockfd);
    return 0;

 }











