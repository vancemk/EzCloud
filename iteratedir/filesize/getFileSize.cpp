#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <dirent.h>

#include <string>
#include <vector>
#include <iostream>

struct PacketHeader {
	char md5Buf[64];
	char pathName[64];	// full path name
	uint32_t change;		// last modify time
	uint32_t fileSize;
	uint32_t lastSync;
};

char *printPwd(){
	int iSize;
	char *pathPwd = (char *)malloc(256);
	if(!getcwd(pathPwd, iSize-1)){
		printf("wrong\n");
	}
	return pathPwd;
}

struct PacketHeader getHeaderInfo(char *fullPathName, struct PackertHeader &ph){
    if (strlen(fullPathName)+1 > 64){
		printf("File name too long. Need to rename the file!!!\n");
		return ph;
	}
	strcpy(ph.pathName, fullPathName);	

	struct stat buf;

    if(lstat(argv[1], &buf)<0){
            printf("wrong\n");
    }
    if(S_ISREG(buf.st_mode)){
        printf("size: %ld\n", buf.st_size);
        printf("access time: %ld\n", buf.st_atime);
        printf("modify time: %ld\n", buf.st_mtime);
        printf("change time: %ld\n", buf.st_ctime);
		ph.fileSize = buf.st_size;
		ph.change = buf.st_ctime;
    }



}


int main(int argc, char **argv){
	
    struct stat buf;

    if(lstat(argv[1], &buf)<0){
            printf("wrong\n");
    }
    if(S_ISREG(buf.st_mode)){
        printf("size: %ld\n", buf.st_size);
        printf("access time: %ld\n", buf.st_atime);
        printf("modify time: %ld\n", buf.st_mtime);
        printf("change time: %ld\n", buf.st_ctime);
    }


    return 0;
}

