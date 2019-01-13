#include <string.h>
#include <stdio.h>
#include <stdint.h>

#ifndef HEAD_H_
#define HEAD_H_

struct Head {
	char strMd5[40];
	char strPathName[128];
	uint64_t fileSize;
	int32_t change;
	int32_t lastSync;
	int8_t isNextFile;
};

void printHead(struct Head *phead){
	printf("phead->strMd5: %s\n", phead->strMd5);
	printf("phead->strPathName: %s\n", phead->strPathName);
	printf("phead->fileSize: %ld\n", phead->fileSize);
	printf("phead->change: %d\n", phead->change);
	printf("phead->lastSync: %d\n", phead->lastSync);
	printf("phead->isNextFile: %d\n\n", phead->isNextFile);
}

struct Head getHead() {
	struct Head temp;
	memcpy(temp.strMd5," 9403e3a5a8fd7b698553e9f0b711cf8d", 32);
	memcpy(temp.strPathName, "/home/Git/EzCloud/databuffer/test_void_convert_struct/databuffer.h", 66);
	temp.fileSize = 10822;
	temp.change = 1542813349;
	temp.lastSync =  0;
	return temp;
}




#endif

