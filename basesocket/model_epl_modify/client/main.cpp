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

#define READ_WRITE_SIZE 1024
using namespace std;

void writeHead(struct Head * phead, DataBuffer & pdbuf, const int pconfd);
void writeFile(struct Head * phead, DataBuffer & pdbuf, const int pconfd);
void writeAll(DataBuffer & pdbuf, const int pconfd) ;
int getConSock();
const char * DEPOTDIR = "./Z_DEPOTDIR/";

int main(void)
 {
	char bufPwdPath[256];
	if (!getcwd(bufPwdPath, 256)) {
		printf("func main: get pwd path wrong, process terminated.\n");
		exit(-1);
	}
	printf("func main: PWD: %s\n\n", bufPwdPath);
	
	int tconnfd = getConSock();

	DataBuffer dbuf, dbuf1;
	dbuf.ensureFree(READ_WRITE_SIZE);
	printf("dbuf.getDataLen: %d\n", dbuf.getDataLen());
	printf("dbuf.getFreeLen: %d\n", dbuf.getFreeLen());
	
	vector<std::string> vecPath;
	vector<struct Head> vecHead;
	iterateDir(DEPOTDIR, vecPath);
    getHeadInfo1(vecHead, vecPath, bufPwdPath);
	
	// starts the first task
	writeTasks(vecHead, dbuf, tconnfd);

	// start to receive from server
	log_msg("start to receive from srv");
	vector<struct Head> vecRtTask; // nextFile=0 rcvd from srv
	sleep(3);
	readTasks(vecRtTask, dbuf, tconnfd);

	log_msg("starts to sleep, ready for last transmission");
	log_msg("vecRtTask.size: %ld", vecRtTask.size());
	writeTasks(vecRtTask, dbuf, tconnfd);
	close(tconnfd);

    return 0;

 }






