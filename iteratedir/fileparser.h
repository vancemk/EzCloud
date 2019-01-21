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

#include "head.h"
#include "md5val.h"

#ifndef "FILEPARSER_H_"
#define "FILEPARSER_H_"

struct Head;
char * print_md5_sum(const char *sFileName, char md5Str[]) ;
using namespace std;

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
char *printPwd(){
	int iSize;
	char *pathPwd = (char *)malloc(256);
	if(!getcwd(pathPwd, iSize-1)){
		printf("wrong\n");
	}
	return pathPwd;
}

void iterateDir(char *inPathName, std::vector<std::string>&vec){
	struct stat buf;
	struct dirent *dirp;
	DIR *dp;

	if(lstat(inPathName, &buf)<0){
		printf("wrong\n");
	}
	if(S_ISDIR(buf.st_mode)){
		dp = opendir(inPathName);
		if(chdir(inPathName)<0){
			printf("param is not a dir\n");
		}
		while((dirp = readdir(dp)) != NULL){
			if(0 == strcmp(dirp->d_name, ".") || 0 == strcmp(dirp->d_name, "..")){
				continue;
			}
			iterateDir(dirp->d_name, vec);
		}
		chdir("..");
	}
	else if(S_ISREG(buf.st_mode)){
		char *pathNow = printPwd();
		char t[256];
		strcpy(t, pathNow);
		vec.push_back(t + std::string("/") +  std::string(inPathName));
		free(pathNow);
	}
	else
		;
}

void getHeadInfo(vector<struct Head>& vecHeads, const vector<string>& vecPaths){
	struct Head tmpHead;
	for (auto &i:vecPaths){
		memset(tmpHead.strPathName, 0, 128);
		memcpy(tmpHead.strPathName, i.c_str(), strlen(i.c_str()));
		vecHeads.push_back(tmpHead);
	}
	for (auto &st:vecHeads){
		st.fileSize = getFileSize(st.strPathName);
		st.change = getModTime(st.strPathName);
		print_md5_sum(st.strPathName, st.strMd5) ;
		st.lastSync = 0;
		printHead(&st);
	}
}

/*
int main(int argc, char **argv){
	vector<std::string> vecPath;
	vector<struct Head> vecHead;
	struct Head tmpHead;
	iterateDir(argv[1], vecPath);
	getHeadInfo(vecHead , vecPath);

	return 0;
}
*/

#endif
