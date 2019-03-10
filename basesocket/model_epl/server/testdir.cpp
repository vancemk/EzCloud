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

#include <string>
#include <vector>

#include "../Heads/databuffer.h"
#include "../Heads/head.h"
#include "../Heads/headhandle.h"
#include "../Heads/fileparser.h"
#include "../Heads/sockcli.h"

#define DEPOTDIR "./Z_DEPOTDIR/"

using namespace std;

int main() {
	char bufPwdPath[256];
	if (!getcwd(bufPwdPath, 256)) {
		printf("func main: get pwd path wrong, process terminated.\n");
		exit(-1);
	}
	printf("func main: PWD: %s\n\n", bufPwdPath);
	vector<std::string> vecPath;
	vector<struct Head> vecHead;
	iterateDir(DEPOTDIR, vecPath);
	// getHeadInfo(vecHead, vecPath);
    getHeadInfo1(vecHead, vecPath, bufPwdPath);

	for (auto i:vecHead) {
		printHead(&i);
	}
}







