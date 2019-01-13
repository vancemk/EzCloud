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
	int datawt = write(pconfd, (void *)pdbuf.getData(), pdbuf.getDataLen());
	pdbuf.clear();
	pdbuf.writeBytes(phead+offset, HEAD_SIZE-offset);
	return;
}

void writeAll(DataBuffer & pdbuf, const pconfd) {
	;
}	









