/**
 * fcntl——manipulate(/məˈnɪpjʊleɪt;) file descriptor
 * **/

#include<sys/types.h>
#include<sys/fcntl.h>
#include<unistd.h>
#include<string.h> 
#include<stdlib.h>
#include<stdio.h>

int main(){

	int fd;
	int val;

	if((fd = open("../../../../../../Documents.tar.gz",O_RDONLY)) == -1){
		    printf("%s", "open failed!\n");
			    return 0;
	}
	//得到进程文件的描述符标志
    printf("filehandle: %d\n",fd);
	if((val = fcntl(fd,F_GETFL,0)) ==-1){
	    printf("fcntl failed!\n");
        return 0;
    }
    printf("%d\n",val);
    printf("%d\n",fd);
	
	switch (val & O_ACCMODE) {   /*单独取出文件读写状态值*/
		case O_RDONLY:
			printf("read only\n");
		    break;
		case O_WRONLY:
			printf("write only\n");
			break;
		case O_RDWR:
			printf("read and write\n");
		break;
	}

    return 0;
}
