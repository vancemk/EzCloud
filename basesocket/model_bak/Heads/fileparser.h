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
#include <setjmp.h>
#include <signal.h>
#define MAXSIZE 4096
#define IP "127.0.0.1"
#define SERV_PORT 8000

#include "databuffer.h"
#include "head.h"
#include "log.h"

#ifndef FILEPARSER_H_
#define FILEPARSER_H_

#define HEAD_SIZE 192

jmp_buf jmpbuf;

void readAll(DataBuffer & pdbuf, const int pconfd) ;

/**
 * @brief  处理 alarm 信号
 **/
void alarmHandler(int sig) {
	siglongjmp(jmpbuf, 1);
	log_msg("handle sigalarm");
}




/**
 * @note 默认样式：path+filename sample: ./Z_DEPOTDIR/
 * @param fullpathname 全路径名称
 * @return <0 出错
 **/
int createDir(const char * fullpathname) {
	struct stat statbuf;
	char strbuf[128];
	if (lstat(fullpathname, &statbuf) >= 0) {
		log_msg("file already exists");
		return 1;
	}
	for (size_t i=10; i<strlen(fullpathname); ++i) {
		if ('/' == fullpathname[i]) {
			memcpy(strbuf, fullpathname, i);
			strbuf[i] = 0;
			if (lstat(strbuf, &statbuf) < 0) {
				if (mkdir(strbuf, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) < 0) {
					log_msg("mkdir failure");
					return -1;
				}
			}
		}
	}

	return 1;
}



/** 
 *  @brief 将缓冲区所有数据写入套接字
 *  @param pdbuf	缓冲区
 *  @param pconfd	打开的连接套接字
 *
 *  @return void 
 */
void writeAll(DataBuffer & pdbuf, const int pconfd) {
    int lenwt = 0;
    while (1) {
        lenwt = write(pconfd, (void *)pdbuf.getData(), 
               pdbuf.getDataLen());
        pdbuf.drainData(lenwt);
        if(0 == pdbuf.getDataLen())
            break;
    }   
}  


/** 
 *  @brief 从缓冲区获取头信息
 *  @param phead	头信息结构指针
 *  @param pdbuf    缓冲区
 *  @param pconfd	打开的连接套接字
 *
 *  @return void 
 */
void readHead(struct Head & rhead, DataBuffer & pdbuf, const int pconfd) {
	if (HEAD_SIZE <= pdbuf.getDataLen()) {
		copyHead(&rhead, (struct Head *)pdbuf.getData());
		pdbuf.drainData(HEAD_SIZE);
		return;
	}
	char tbuf[HEAD_SIZE];
	int offset = pdbuf.getDataLen();
	memcpy(tbuf, (void *)pdbuf.getData(), pdbuf.getDataLen());
	pdbuf.clear();
	readAll(pdbuf, pconfd);
	memcpy(tbuf+offset, (void *)pdbuf.getData(), HEAD_SIZE-offset);
	copyHead(&rhead, (struct Head*)tbuf);
	pdbuf.drainData(HEAD_SIZE-offset);
	return;
}


/** 
 *  @brief 从缓冲区获取头信息 -- non-block
 *  @param phead	头信息结构指针
 *  @param pdbuf    缓冲区
 *  @param pconfd	打开的连接套接字
 *
 *  @return void 
 */
void readHead1(struct Head & rhead, DataBuffer & pdbuf, const int pconfd) {
	if (HEAD_SIZE <= pdbuf.getDataLen()) {
		copyHead(&rhead, (struct Head *)pdbuf.getData());
		pdbuf.drainData(HEAD_SIZE);
		return;
	}
	char tbuf[HEAD_SIZE];
	int offset = pdbuf.getDataLen();
	memcpy(tbuf, (void *)pdbuf.getData(), pdbuf.getDataLen());
	read(pconfd, (void *) (tbuf + offset), HEAD_SIZE-offset);
	copyHead(&rhead, (struct Head*)tbuf);
	return;
}



/** 
 *  @brief 向缓冲区写入头信息
 *  @param phead	头信息结构指针
 *  @param pdbuf    缓冲区
 *  @param pconfd	打开的连接套接字
 *
 *  @return void 
 */
void writeHead(struct Head * phead, DataBuffer & pdbuf, const int pconfd) {
	if (pdbuf.getFreeLen() < 0){
		exit(-1);
	}
	log_msg("send Head: %s", phead->strPathName);
	int offset = pdbuf.getFreeLen() >= HEAD_SIZE ? HEAD_SIZE : pdbuf.getFreeLen();
	if (pdbuf.getFreeLen() >= HEAD_SIZE) {
		pdbuf.writeBytes(phead, HEAD_SIZE);	
		return;
	}
	else {
		pdbuf.writeBytes(phead, offset);
	}
	writeAll(pdbuf, pconfd); 
	pdbuf.writeBytes((char *)phead+offset, HEAD_SIZE-offset);
	return;
}




/** 
 *  @brief 从套接字读入数据填满缓冲区
 *  @param pdbuf	缓冲区
 *  @param pconfd	打开的连接套接字
 *
 *  @return void 
 */
void readAll(DataBuffer & pdbuf, const int pconfd) {
	signal(SIGALRM, alarmHandler);
    int lenrd = 0;
	if (0 == pdbuf.getFreeLen() || 0 != pdbuf.getDataLen()){
		return;
	}
    while (1) {
		alarm(6);
        lenrd = read(pconfd, (void *)pdbuf.getFree(), 
               pdbuf.getFreeLen());
		if (sigsetjmp(jmpbuf, 1) != 0) {
			// log_msg("sigsetjmp test");
			break;
		}
		pdbuf.pourData(lenrd);
		if (0 >= lenrd || 0 == pdbuf.getFreeLen()){
			break;
		}
    }   
}   


/** 
 *  @brief 从套接字读入数据填满缓冲区
 *  @param pdbuf	缓冲区
 *  @param pconfd	打开的连接套接字
 *
 *  @return void 
 */
void readAll(DataBuffer & pdbuf, const int pconfd,
		const int waitseconds) {
	signal(SIGALRM, alarmHandler);
    int lenrd = 0;
	if (0 == pdbuf.getFreeLen() || 0 != pdbuf.getDataLen()){
		return;
	}
    while (1) {
		alarm(waitseconds);
        lenrd = read(pconfd, (void *)pdbuf.getFree(), 
               pdbuf.getFreeLen());
		if (sigsetjmp(jmpbuf, 2) != 0) {
			log_msg("sigsetjmp test");
			break;
		}
		pdbuf.pourData(lenrd);
		if (0 >= lenrd || 0 == pdbuf.getFreeLen()){
			break;
		}
    }   
}   




/** 
 *  @brief 从缓冲区读数据并写文件  
 *  @param phead	头信息结构指针
 *  @param pdbuf    缓冲区
 *  @param pconfd	打开的连接套接字
 *
 *  @return void 
 */
void readFile(struct Head * phead, DataBuffer & pdbuf, const int pconfd) {
	if (pdbuf.getFreeLen() < 0){
		exit(-1);
	}
	if (createDir(phead->strPathName) < 0){
		log_msg("create dir: %s failed", phead->strPathName);
		exit(-1);
	}
	log_msg("recv file: %s", phead->strPathName);
	int tfd = open(phead->strPathName, O_CREAT | O_WRONLY);
	ulong tFileSize = (ulong)phead->fileSize;
	int lenrd = 0;
	if (tfd < 0)
		exit(-1);
	while (1) {
		if (tFileSize <= (ulong)pdbuf.getDataLen()) {
			lenrd = write(tfd, (void *)pdbuf.getData(), 
				tFileSize);
			pdbuf.drainData(lenrd);
			tFileSize -= lenrd ;
		}
		else {
			lenrd = write(tfd, (void *)pdbuf.getData(), 
				pdbuf.getDataLen());
			pdbuf.drainData(lenrd);
			tFileSize -= lenrd;
		}
		if (tFileSize > 0 && 0 == pdbuf.getFreeLen()) {
			readAll(pdbuf, pconfd);
		}
		if (0 >=  lenrd ||  tFileSize == 0)
			break;
		readAll(pdbuf, pconfd); 
	}
}



/** 
 *  @brief 向缓冲区写入文件信息
 *  @param phead	头信息结构指针
 *  @param pdbuf    缓冲区
 *  @param pconfd	打开的连接套接字
 *
 *  @return void 
 */
void writeFile(struct Head * phead, DataBuffer & pdbuf, const int pconfd) {
	if (pdbuf.getFreeLen() < 0){
		exit(-1);
	}
	log_msg("send file: %s", phead->strPathName);
	int tfd = open(phead->strPathName, O_RDONLY);
	int lenrd = 0;
	if (tfd < 0)
		exit(-1);
	while (1) {
		lenrd = read(tfd, (void *)pdbuf.getFree(), 
				pdbuf.getFreeLen());
		pdbuf.pourData(lenrd);
		if (0 >=  lenrd) {
			close(tfd);
			break;
		}
		writeAll(pdbuf, pconfd); 
	}
}

/**
 *
 *
 **/
void writeTasks(std::vector<struct Head>& vecTasks, DataBuffer & pdbuf,
		const int pconfd) {
	pdbuf.clear();
	for (ulong i=0; i<vecTasks.size(); i++){
		// vecTasks[i].isNextFile = 0;
		writeHead(&vecTasks[i], pdbuf, pconfd);
		if (1 == vecTasks[i].isNextFile) {
			writeFile(&vecTasks[i], pdbuf, pconfd);
		}   
	}   
	vecTasks[0].isNextFile = -1; 
	writeHead(&vecTasks[0], pdbuf, pconfd);
	writeAll(pdbuf, pconfd);
}

/**
 *
 *
 **/
void readTasks(std::vector<struct Head>& vecTasks, DataBuffer & pdbuf,
		const int pconfd) {
	pdbuf.clear();
    readAll(pdbuf, pconfd);
	struct Head testHead;
	while(1) {
		readHead(testHead, pdbuf, pconfd);
	
		if (1 == testHead.isNextFile) {
			readFile(&testHead, pdbuf, pconfd);
		}   
		else if (0 == testHead.isNextFile ||  2 == testHead.isNextFile) {
			if (2 == testHead.isNextFile) {
				testHead.isNextFile = 1;
			}
			struct Head tmphead;
			vecTasks.push_back(tmphead);
			copyHead(&vecTasks[vecTasks.size()-1], &testHead);
		}   
		else if (-1 == testHead.isNextFile) {
			log_msg("rcv task-finished flag");
			break;
		}   
													        // sleep(1);
	}												//     }
}

#endif
