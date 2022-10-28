#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<netinet/in.h>
#include<errno.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<time.h>
#include <cstdlib>
#define EPOLLEVENTS 100
#define MAXSIZE 1024
class EpollBase
{
private:
    /* data */
public:


    int socket_bind(const char *ip,int port);
    void do_epoll(int listenfd);
    void handle_events(int epollfd,struct epoll_event *events,int num,int listenfd,char *buf);
    void handle_accept(int epollfd,int listenfd);
    void do_read(int epollfd,int fd,char *buf);
    void do_write(int epollfd,int fd,char *buf);
    void add_events(int epollfd,int fd ,int state);
    void modify_events(int epollfd,int fd ,int state);
    void delete_events(int epollfd,int fd ,int state);

};


