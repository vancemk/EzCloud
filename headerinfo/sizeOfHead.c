#include <string.h>
#include <stdio.h>
#include <stdint.h>

struct head {
	char strMd5[40];
	char strPathName[128];
	uint64_t fileSize;
	int32_t change;
	int32_t lastSync;
};

int main(){

	printf("sizeof struct head: %ld\n", sizeof(struct head));


	return 0;

}

