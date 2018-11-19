#include <stdio.h>
#include <string.h>

int main(){

	char buf[64];
	sprintf(buf, "%c", 'h');
	buf[11] = 0;
	printf("buf: %s\n", buf);


}
