#include <string.h>
#include <stdio.h>


struct head {
	char strMd5[64];
	long change;
	char strPathName[128];
	int fileSize;
	int lastSync;
};



