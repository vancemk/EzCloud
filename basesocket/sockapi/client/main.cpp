/*************************************************************************
    * File Name: client.c
    * Author:    The answer
    * Function:  Other        
    * Mail:      2412799512@qq.com 
    * Created Time: 2017年06月15日 星期四 18时54分22秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<errno.h>
#include<unistd.h>
#define MAXSIZE 4096
#define IP "127.0.0.1"
#define SERV_PORT 8000

#include "head.h"
#include "sockcli.h"
#include "databuffer.h"

void sys_err(const char *ptr,int num);
int getConSock();
//struct Head;
void printHead(struct Head *phead);


int main(int argc,char **argv)
 {
    int sockfd = getConSock();
	DataBuffer dbuf;


	// start to init test struct
	struct Head testHead, rHead;
	testHead.change = 123456;
	testHead.fileSize = 0;
	memcpy(testHead.strPathName,"/home",5);
	memcpy(testHead.strMd5,"abcdefghijklmnopqrstuvwxyzxxxxxx", 32);
	testHead.strMd5[32] = 0;
	testHead.strPathName[5]= 0;

	// printf("ptr #####: %p\n", &(testHead.strMd5));
	// binary write struct to dbuf
	dbuf.writeBytes((void *)&testHead, sizeof(testHead));
	dbuf.writeBytes((void *)&testHead, sizeof(testHead));
	printf("dbuf data len: %d \n", dbuf.getDataLen());
    while(1)
    {
		int len = dbuf.getDataLen();
        if(len == 0)
            break;

        int _tmp = 0;
        //考虑若socket缓冲区小于len的情况，见原理图1
        while(1)
        {
            int ret = write(sockfd, dbuf.getData()+_tmp, len - _tmp);
			// printf("write: %d\n", ret);
            if(ret > 0 )
                _tmp += ret;
            if(_tmp == ret)
				len = 0;
                break;
            if(ret < 0)
            {
                perror("write");
                break;
            }
        }
		if (len == 0)
			break;

    }
	// printf("dbuf.getData: %s\n", dbuf.getData());
	// printf("dbuf.getDataLen: %d\n", dbuf.getDataLen());
	dbuf.readBytes((void *)&rHead, sizeof(testHead));
	printHead(&rHead);
	// printf("ptr #####: %p\n", &(rHead.strMd5));
    close(sockfd);
    return 0;
 }


