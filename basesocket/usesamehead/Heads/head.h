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

struct Head * copyHead(struct Head * lhead, struct Head * rhead) {
	memcpy(lhead->strMd5, rhead->strMd5, 40);
	memcpy(lhead->strPathName, rhead->strPathName, 128);
	lhead->fileSize = rhead -> fileSize;
	lhead->change= rhead -> change;
	lhead->lastSync = rhead -> lastSync;
	lhead->isNextFile = rhead -> isNextFile;

	return lhead;
}

#endif
