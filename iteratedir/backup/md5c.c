#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <openssl/md5.h>

unsigned char result[MD5_DIGEST_LENGTH];

// Get the size of the file by its file descriptor
unsigned long get_size_by_fd(int fd) {
    struct stat statbuf;
    if(fstat(fd, &statbuf) < 0) exit(-1);
    return statbuf.st_size;
}
// Print the MD5 sum as hex-digits.
char * print_md5_sum(const char *sFileName, char md5Str[]) {
    int tfd = open(sFileName, O_RDONLY);
	if (tfd < 0) exit(-1);
	

	unsigned char result1[MD5_DIGEST_LENGTH];
    unsigned long fileSize = get_size_by_fd(tfd);
	char * tfilebuffer = mmap(0, fileSize, PROT_READ, MAP_SHARED, tfd, 0);
    MD5((unsigned char*) tfilebuffer, fileSize, result1);
    munmap(tfilebuffer, fileSize); 
	
	memset(md5Str, 0, 32);
	md5Str[32] = 0;
	int i;
    for(i=0; i <MD5_DIGEST_LENGTH; i++) {
            //printf("%02x",result1[i]);
			sprintf(md5Str+i*2, "%02x", result1[i]);
    }
	return md5Str;
}

// Print the MD5 sum as hex-digits.
void print_md5_sum2(const char *sFileName) {
    int tfd = open(sFileName, O_RDONLY);
	if (tfd < 0) exit(-1);
	

	unsigned char result1[MD5_DIGEST_LENGTH];
    unsigned long fileSize = get_size_by_fd(tfd);
	char * tfilebuffer = mmap(0, fileSize, PROT_READ, MAP_SHARED, tfd, 0);
    MD5((unsigned char*) tfilebuffer, fileSize, result1);
    munmap(tfilebuffer, fileSize); 

    char buf[64];
	buf[32] = 0;
	int i;
    for(i=0; i <MD5_DIGEST_LENGTH; i++) {
            //printf("%02x",result1[i]);
			sprintf(buf+i*2, "%02x", result1[i]);
    }
	printf("%s\n", buf);
}
int main(int argc, char *argv[]) {
	print_md5_sum2(argv[1]);

	char ttbuf[64];
	print_md5_sum(argv[1], ttbuf);
	printf(ttbuf);
	printf("\n");

    return 0;
}
