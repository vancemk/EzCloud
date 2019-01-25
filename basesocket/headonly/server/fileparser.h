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

#include "databuffer.h"
#include "head.h"

#ifndef FILEPARSER_H_
#define FILEPARSER_H_

#define HEAD_SIZE 192


void readAll(DataBuffer & pdbuf, const int pconfd) ;
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
        pdbuf.stripData(lenwt);
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
void readHead1(struct Head & rhead, DataBuffer & pdbuf, const int pconfd) {
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
 *  @brief 从套接字读入数据填满缓冲区
 *  @param pdbuf	缓冲区
 *  @param pconfd	打开的连接套接字
 *
 *  @return void 
 */
void readAll(DataBuffer & pdbuf, const int pconfd) {
    int lenrd = 0;
	if (0 == pdbuf.getFreeLen() || 0 != pdbuf.getDataLen()){
		return;
	}
    while (1) {
        lenrd = read(pconfd, (void *)pdbuf.getFree(), 
               pdbuf.getFreeLen());
		pdbuf.pourData(lenrd);
		if (0 >= lenrd) {
			close(pconfd);
		}
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
	int tfd = open(phead->strMd5, O_CREAT | O_WRONLY);
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
 *  @brief 从缓冲区获取头信息
 *  @param phead	头信息结构指针
 *  @param pdbuf    缓冲区
 *  @param pconfd	打开的连接套接字
 *
 *  @return void 
 */
void readHead(struct Head & rhead, DataBuffer & pdbuf) {
	struct Head *thead = (struct Head *)pdbuf.getData();
	memcpy(rhead.strMd5, thead->strMd5, 40);
	memcpy(rhead.strPathName, thead->strPathName, 128);
	rhead.fileSize = thead -> fileSize;
	rhead.change= thead -> change;
	rhead.lastSync = thead -> lastSync;
	rhead.isNextFile = thead -> isNextFile;
	pdbuf.drainData(HEAD_SIZE);
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
	int offset = pdbuf.getFreeLen() >= HEAD_SIZE ? HEAD_SIZE : pdbuf.getFreeLen();
	if (pdbuf.getFreeLen() >= HEAD_SIZE) {
		pdbuf.writeBytes(phead, HEAD_SIZE);	
		return;
	}
	else {
		pdbuf.writeBytes(phead, offset);
	}
	writeAll(pdbuf, pconfd); 
	pdbuf.writeBytes(phead+offset, HEAD_SIZE-offset);
	return;
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
	int tfd = open(phead->strPathName, O_RDONLY);
	int lenrd = 0;
	if (tfd < 0)
		exit(-1);
	while (1) {
		lenrd = read(tfd, (void *)pdbuf.getFree(), 
				pdbuf.getFreeLen());
		pdbuf.pourData(lenrd);
		if (0 ==  lenrd && pdbuf.getFreeLen() != 0)
			break;
		writeAll(pdbuf, pconfd); 
	}
}







#endif
