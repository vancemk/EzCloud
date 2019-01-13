#include <string.h>
#include <stdio.h>
#include <stdint.h>

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


