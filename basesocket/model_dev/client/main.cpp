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
	// getHeadInfo(vecHead, vecPath);
    getHeadInfo1(vecHead, vecPath, bufPwdPath);
	
	// starts the first task
	for (ulong i=0; i<vecHead.size(); i++){
		// vecHead[i].isNextFile = i % 2;
		vecHead[i].isNextFile = 0;
		printHead(&vecHead[i]);
		writeHead(&vecHead[i], dbuf, tconnfd);
		if (1 == vecHead[i].isNextFile) {
			writeFile(&vecHead[i], dbuf, tconnfd);
		}
		// sleep(1);
	}
	vecHead[0].isNextFile = -1;
	writeHead(&vecHead[0], dbuf, tconnfd);
	writeAll(dbuf, tconnfd);

	// start to receive from server
	log_msg("start to receive from srv");
	struct Head testHead;
	vector<struct Head> vecRtTask; // nextFile=0 rcvd from srv
	dbuf.clear();
	log_msg("fcntl tconnfd: %d", fcntl(tconnfd, F_GETFL, 0));


	readAll(dbuf, tconnfd, 15);
	log_msg("dbuf.size %d", dbuf.getDataLen());
	while(1) {
		readHead(testHead, dbuf, tconnfd);
		
		if (1 == testHead.isNextFile) {
			readFile(&testHead, dbuf, tconnfd);
		}
		else if (0 == testHead.isNextFile) {
			struct Head tmphead;
			vecRtTask.push_back(tmphead);
			copyHead(&vecRtTask[vecRtTask.size()-1], &testHead);
		}
		else if (-1 == testHead.isNextFile) {
			log_msg("rcv task-finished flag");
			break;
		}

		printPathName(&testHead);
		if (fcntl(tconnfd, F_GETFL, 0) < 0) {
			log_msg("connected fd has closed");
			break;
		}
		else {
			log_msg("fcntl getfl: %d", fcntl(tconnfd, F_GETFL, 0));
		}
		// sleep(1);
	}
	log_msg("starts to sleep, ready for last transmission");
	sleep(65);
	log_msg("sleep fininshed");

    return 0;

 }






