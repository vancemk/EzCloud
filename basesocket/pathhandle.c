#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define log_msg(fmt, LOG_ARG...)  \
	    printf("[%s:%d][%s]: " fmt "\n", __FILE__, __LINE__, __FUNCTION__, ##LOG_ARG)


int createDir(const char * fullpathname) ;

int main(void) {
	int rtval = mkdir("./hac/", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	log_msg("mkdir return: %d", rtval);

	createDir("/home/Documents/test/PathHandle/hac/a/b/c");

	return 0;
}


/**
 * @note 默认样式：path+filename sample: ./Z_DEPOTDIR/
 * @param fullpathname 全路径名称
 * @return <0 出错
 **/
int createDir(const char * fullpathname) {
	struct stat statbuf;
	char strbuf[128];
	if (lstat(fullpathname, &statbuf) >= 0) {
		log_msg("file already exists");
		return 1;
	}
	for (size_t i=10; i<strlen(fullpathname); ++i) {
		if ('/' == fullpathname[i]) {
			memcpy(strbuf, fullpathname, i);
			strbuf[i] = 0;
			if (lstat(strbuf, &statbuf) < 0) {
				if (mkdir(strbuf, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) < 0) {
					log_msg("mkdir failure");
					return -1;
				}
			}
		}
	}

	return 1;
}




