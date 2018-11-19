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



unsigned long getFileSize(const char *sFileName){
	struct stat statbuf;
	if(lstat(sFileName, &statbuf) < 0){
		printf("wrong\n");
	}
	if(S_ISREG(statbuf.st_mode))
		return statbuf.st_size;
	else
		return 0;
}

unsigned long getModTime(const char *sFileName){
	struct stat statbuf;
	if(lstat(sFileName, &statbuf) < 0){
		printf("wrong\n");
	}
	if(S_ISREG(statbuf.st_mode))
		return statbuf.st_mtime;
	else
		return 0;
}

