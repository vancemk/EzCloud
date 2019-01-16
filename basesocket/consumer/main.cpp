#include "head.h"
#include "databuffer.h"
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

const int HEAD_SIZE 192
static int offset = 0;
using namespace std;

int main(void) {
	struct Head tHead;
	memcpy(tHead.strMd5, "9403e3a5a8fd7b698553e9f0b711cf8d", 32);
	memcpy(tHead.strPathName, "/home/Documents/test/config.h", 29);
	tHead.fileSize = 9123;
	tHead.change = 0;
	tHead.lastSync = 0;
	tHead.isNextFile = 0;
	printHead(&tHead);
	cout << "sizeof: " << sizeof(tHead) << endl;

	return 0;
}


/** 
 *  @brief 向缓冲区写入头信息
 *  @param phead	头信息结构指针
 *  @param pdbuf    缓冲区
 *  @param pconfd	打开的连接套接字
 *
 *  @return void 
 */
void writeHead(struct Head * phead, DataBuffer & pdbf, const int pconfd) {
	if (pbuf.getFreeLen() < 0){
		return -1;
	}
	int offset = pdbuf.getFreeLen >= HEAD_SIZE ? HEAD_SIZE : pdbuf.getFreeLen();
	if (pdbuf.getFreeLen >= HEAD_SIZE) {
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
void writeFile(struct Head * phead, DataBuffer & pdbf, const int pconfd) {
	if (pbuf.getFreeLen() < 0){
		return -1;
	}
	int tfd = open(pthed->sFileName, O_RDONLY);
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
        int lenwt = write(pconfd, (void *)pdbuf.getData(), 
               pdbuf.getDataLen());
        pdbuf.stripData(lenwt);
        if(0 == pdbuf.getDataLen())
            break;
    }   
}   










