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
void print_md5_sum(unsigned char* md) {
    int i;
    for(i=0; i <MD5_DIGEST_LENGTH; i++) {
            printf("%02x",md[i]);
    }
}

// Print the MD5 sum as hex-digits.
void print_md5_sum1(const char *sFileName) {
    int tfd = open(sFileName, O_RDONLY);
	if (tfd < 0) exit(-1);
	

	unsigned char result1[MD5_DIGEST_LENGTH];
    unsigned long fileSize = get_size_by_fd(tfd);
	char * tfilebuffer = mmap(0, fileSize, PROT_READ, MAP_SHARED, tfd, 0);
    MD5((unsigned char*) tfilebuffer, fileSize, result1);
    munmap(tfilebuffer, fileSize); 


	int i;
    for(i=0; i <MD5_DIGEST_LENGTH; i++) {
            printf("%02x",result1[i]);
    }
}

int main(int argc, char *argv[]) {
	print_md5_sum1(argv[1]);
    return 0;
}
