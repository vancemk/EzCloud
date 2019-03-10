#include <unistd.h>
#include <sys/epoll.h>

#include "databuffer.h"
#include "fileparser.h"
#include "head.h"
#include "log.h"
#include "md5val.h"
#include "sockser.h"
#include "headhandle.h"

#ifndef EPOLLSOCKETEVENT_H_
#define EPOLLSOCKETEVENT_H_

#define DEPOTDIR "./Z_DEPOTDIR/"

class EPollSocketEvent {

public:
    /*
     * 构造函数
     */
	EPollSocketEvent(const int listenfd, DataBuffer &dbuf):_listenfd(listenfd),pdbuf(dbuf) {
		_iepfd = epoll_create(MAX_SOCKET_EVENTS);
		pdbuf.ensureFree(2048);
	}
    /*
     * 析造函数
     */
    ~EPollSocketEvent() {
		close(_iepfd);
		close(_listenfd);
	}
    /*
     * 增加Socket到事件中
     *
     * @param socket 被加的socket
     * @param enableRead: 设置是否可读
     * @param enableWrite: 设置是否可写
     * @return  操作是否成功, true – 成功, false – 失败
     */
	bool addEvent(const int sockfd, bool enableRead, bool enableWrite) {

		struct epoll_event ev;
		memset(&ev, 0, sizeof(ev));
		ev.data.fd = sockfd;
		// 设置要处理的事件类型
		ev.events = 0;

		if (enableRead) {
			ev.events |= EPOLLIN;
		}
		if (enableWrite) {
			ev.events |= EPOLLOUT;
		}

		bool rc = (epoll_ctl(_iepfd, EPOLL_CTL_ADD, sockfd, &ev) == 0);
		return rc;
	}

    /*
     * 设置删除Socket到事件中
     *
     * @param socket 被加的socket
     * @param enableRead: 设置是否可读
     * @param enableWrite: 设置是否可写
     * @return  操作是否成功, true – 成功, false – 失败
     */
	bool setEvent(const int sockfd, bool enableRead, bool enableWrite) {

		struct epoll_event ev;
		memset(&ev, 0, sizeof(ev));
		ev.data.fd = sockfd;
		// 设置要处理的事件类型
		ev.events = 0;

		if (enableRead) {
			ev.events |= EPOLLIN;
		}
		if (enableWrite) {
			ev.events |= EPOLLOUT;
		}

		bool rc = (epoll_ctl(_iepfd, EPOLL_CTL_MOD, sockfd, &ev) == 0);
		return rc;
	}
    /*
     * 删除Socket到事件中
     *
     * @param socket 被删除socket
     * @return  操作是否成功, true – 成功, false – 失败
     */
	bool removeEvent(const int sockfd) {

		struct epoll_event ev;
		memset(&ev, 0, sizeof(ev));
		ev.data.fd = sockfd;
		// 设置要处理的事件类型
		ev.events = 0;
		//_mutex.lock();
		bool rc = (epoll_ctl(_iepfd, EPOLL_CTL_DEL, sockfd, &ev) == 0);
		//_mutex.unlock();

		return rc;
	}
	
	
	void accHandle() {
		int connfd;
		struct sockaddr_in cliaddr;
		socklen_t  cliaddrlen;
		bzero(&cliaddr, sizeof(cliaddr));
		


		connfd = accept(_listenfd,(struct sockaddr*)&cliaddr,&cliaddrlen);
		if (connfd == -1)
			log_msg("accpet error:");
		else
		{
			log_msg("accept a new client: %s:%d\n",inet_ntoa(cliaddr.sin_addr),cliaddr.sin_port);
			//添加一个客户描述符和事件
			addEvent(connfd, true, false) ;
		}
	}
	
    void eventLoop(){
	    struct epoll_event events[MAX_SOCKET_EVENTS];

    	int res;
	    while (1) {
    	    // 检查是否有事件发生
	        res =  epoll_wait(_iepfd, events, MAX_SOCKET_EVENTS , -1);
    	    eventHandle(events, res);
        }
}

	void eventHandle(struct epoll_event *events, int cnt) {
	

		int tfd;
		//进行选好遍历
		for (int i=0;i < cnt; i++)
		{
			tfd = events[i].data.fd;
			//根据描述符的类型和事件类型进行处理
			if ((tfd == _listenfd) &&(events[i].events & EPOLLIN)){

				accHandle();
				log_msg("tfd= %d, _listenfd= %d", tfd, _listenfd);
			}
			else if (events[i].events & EPOLLIN) {
				
				log_msg("now tfd is: %d", tfd);
				std::vector<struct Head> vecRmtHeads, vecLocHeads;
				readTasks(vecRmtHeads, pdbuf, tfd);
				log_msg("vecRmtHeads size: %ld", vecRmtHeads.size());
				if(!vecRmtHeads.empty()) {
					if (0 == vecRmtHeads[0].isNextFile) {
						char bufPwdPath[256];
						if (!getcwd(bufPwdPath, 256)) {
							printf("get pwd path wrong, process terminated.\n");
							exit(-1);
						}
						printf("func main: PWD: %s\n\n", bufPwdPath);
						std::vector<std::string> vecPath;
						iterateDir(DEPOTDIR, vecPath);
					    getHeadInfo1(vecLocHeads, vecPath, bufPwdPath);
						log_msg("vecLocalHeads.size: %ld", vecLocHeads.size());
						cmpVecInfos(vecLocHeads, vecRmtHeads, vecTasks);
						log_msg("vecLocHeads size: %ld\nvecRmtHeads size: %ld\nvec\
								DiffHeads size: %ld\n", vecLocHeads.size(), 
								vecRmtHeads.size(),	vecTasks.size());
					}
					else {
						for (auto &i:vecRmtHeads){
							printHead(&i);
						}
						removeEvent(tfd);
					}
				}
				setEvent(tfd, 0, 1); // set write
			}
			else if (events[i].events & EPOLLOUT) {
				log_msg("vecTasks size: %ld", vecTasks.size());
				writeTasks(vecTasks, pdbuf, tfd);
				vecTasks.clear();
				setEvent(tfd, 1, 0);
			}
		}
	}

private:
    int _iepfd;    // epoll的fd
    const int _listenfd;    // listenfd
    const int MAX_SOCKET_EVENTS = 1024;
	std::vector<struct Head> vecTasks;
	DataBuffer & pdbuf;
};


#endif /*EPOLLSOCKETEVENT_H_*/
