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
#include <algorithm>

#include "head.h"
#include "md5val.h"
#include "log.h"

#ifndef HEADHANDLE_H_
#define HEADHANDLE_H_

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
	int iSize =256;
	char *pathPwd = (char *)malloc(256);
	if(!getcwd(pathPwd, iSize-1)){
		printf("wrong\n");
	}
	// printf("func printPwd: %s\n", pathPwd);
	return pathPwd;
}

void iterateDir(const char *inPathName, std::vector<std::string>&vec){
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


/** 
 *  @brief 补全 strcut Head 中其他信息
 *  @param vecHeads    文件全路径信息
 *  @param vecPaths    返回的补全的 struct Heads vector
 *	@param inWorkPath  传入的工作路径(后期会置为默认)
 *  @return void 
 */
void getHeadInfo1(vector<struct Head>& vecHeads, vector<string>& vecPaths, 
		const char * dftDepotDir){
	struct Head tmpHead;
	for (size_t i=0; i<vecPaths.size(); i++){
		vecPaths[i].replace(0, strlen(dftDepotDir), ".");
		// printf("func getHeadInfo1: i: %s\n", vecPaths[i].c_str());
		memset(tmpHead.strPathName, 0, 128);
		memcpy(tmpHead.strPathName, vecPaths[i].c_str(), strlen(vecPaths[i].c_str()));
		vecHeads.push_back(tmpHead);
	}
	for (size_t st=0; st<vecHeads.size(); ++st){
		vecHeads[st].fileSize = getFileSize(vecHeads[st].strPathName);
		vecHeads[st].change = getModTime(vecHeads[st].strPathName);
		print_md5_sum(vecHeads[st].strPathName, vecHeads[st].strMd5) ;
		vecHeads[st].lastSync = 0;
	}
}


void getHeadInfo(vector<struct Head>& vecHeads, const vector<string>& vecPaths){
	struct Head tmpHead;
	for (size_t i=0; i<vecPaths.size(); ++i){
		memset(tmpHead.strPathName, 0, 128);
		memcpy(tmpHead.strPathName, vecPaths[i].c_str(), strlen(vecPaths[i].c_str()));
		vecHeads.push_back(tmpHead);
	}
	for (size_t st=0; st<vecHeads.size(); ++st){
		vecHeads[st].fileSize = getFileSize(vecHeads[st].strPathName);
		vecHeads[st].change = getModTime(vecHeads[st].strPathName);
		print_md5_sum(vecHeads[st].strPathName, vecHeads[st].strMd5) ;
		vecHeads[st].lastSync = 0;
	}
}


/**
 * @brief  对比双端头信息 vector 生成任务 vector
 * @param vecLocHeads  本地文件信息所生成的 struct Head vector
 * @param vecRmtHeads  
 **/
void cmpVecInfos(vector<struct Head> & vecLocHeads,
		vector<struct Head> & vecRmtHeads, vector<struct Head> & vecRtDiffs) {
	vecRtDiffs.clear();
	bool hasSameHead = false;
	for(size_t i=0; i<vecLocHeads.size(); ++i) {
		for (size_t h=0; h<vecRmtHeads.size(); ++h) {
			if (ifHeadsEqual(vecLocHeads[i], vecRmtHeads[h])) {
				hasSameHead = true;
				break;
			}				
		}
		if (!hasSameHead) {
			vecLocHeads[i].isNextFile = 1;
			struct Head tmphead;
			vecRtDiffs.push_back(tmphead);
			copyHead(&vecRtDiffs[vecRtDiffs.size()-1], &vecLocHeads[i]);
		}
		else {
			// log_msg("local and rmt has same head");
			// printPathName(&vecLocHeads[i]);
			hasSameHead = false;
		}
	}
	
	for (size_t h=0; h<vecRmtHeads.size(); ++h) {
		for(size_t i=0; i<vecLocHeads.size(); ++i) {
			if (ifHeadsEqual(vecLocHeads[i], vecRmtHeads[h])) {
				hasSameHead = true;
				break;
			}
		}
		if (!hasSameHead) {
			vecLocHeads[h].isNextFile = 0;
			struct Head tmphead;
			vecRtDiffs.push_back(tmphead);
			copyHead(&vecRtDiffs[vecRtDiffs.size()-1], &vecRmtHeads[h]);
		}
		else {
			hasSameHead = false;
		}
	}
	log_msg("vecRtDiffs size: %ld\n", vecRtDiffs.size());
	// for (auto i:vecRtDiffs){
	//	log_msg("vecRtDiffs %s", i.strPathName);
	// }
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


