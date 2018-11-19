#include <string.h>
#include <stdio.h>
#include <stdint.h>

struct Head {
	char strMd5[40];
	char strPathName[128];
	uint64_t fileSize;
	int32_t change;
	int32_t lastSync;
};

void printHead(struct Head *phead){
	printf("phead->strMd5: %s\n", (char *)phead->strMd5);
	printf("phead->strPathName: %s\n", (char *)phead->strPathName);
	printf("phead->fileSize: %ld\n", phead->fileSize);
	printf("phead->strPathName: %d\n", phead->change);
	printf("phead->strPathName: %d\n", phead->lastSync);
}


