#include "databuffer.h"
#include <stdio.h>
#include <stdint.h>
#include <iostream>

using namespace std;

struct Head {
	uint32_t lenName;
};


int main(){
	DataBuffer dbuf;
	printf("get data len: %d\n", dbuf.getDataLen());

	struct Head reqHead, rspHead;
	reqHead.lenName = 22;

	dbuf.writeBytes((void *)&reqHead, sizeof(reqHead));

	printf("get data len: %d\n", dbuf.getDataLen());

	dbuf.readBytes((void *)&rspHead, sizeof(reqHead));
	printf("%d\n", rspHead.lenName);
	return 0;

}








