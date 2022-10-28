#include"server.h"
int connects=0;
int EpollBase::socket_bind(const char *ip,int port)
{
    int listenfd;
    listenfd=socket(AF_INET,SOCK_STREAM,0);
    if(listenfd==-1)
    {
        printf("socket error!\n");
        exit(1);
    }
    struct sockaddr_in servAdr;
    bzero(&servAdr,sizeof(servAdr));
    servAdr.sin_family=AF_INET;
    inet_pton(AF_INET,ip,&servAdr.sin_addr);
    servAdr.sin_port=htons(port);
    if(bind(listenfd,(sockaddr *)&servAdr,sizeof(servAdr))==-1)
    {
        printf("bind error!\n");
        exit(1);
    }else
    {
        
        printf("bind sucess!\n");
    }

    return listenfd;
}
void EpollBase::do_epoll(int listenfd)
{
    int epollfd;
    epoll_event events[EPOLLEVENTS];
    char buf[MAXSIZE];
    memset(buf,0,MAXSIZE);
    epollfd=epoll_create(1000);
    add_events(epollfd,listenfd,EPOLLIN);
    while(1)
    {
        int ret=epoll_wait(epollfd,events,EPOLLEVENTS,-1);
        handle_events(epollfd,events,ret,listenfd,buf);
    }
    close(epollfd);
}
void EpollBase::handle_events(int epollfd,struct epoll_event *events,int num,int listenfd,char *buf)
{
    int i,fd;
    for(i=0;i<num;i++)
    {
        fd=events[i].data.fd;
        if(fd==listenfd&&(events[i].events&EPOLLIN))
            handle_accept(epollfd,listenfd);
        else if(events[i].events&EPOLLIN)
            do_read(epollfd,events[i].data.fd,buf);
        else if(events[i].events&EPOLLOUT)
            do_write(epollfd,events[i].data.fd,buf);
            
    }
}
void EpollBase::handle_accept(int epollfd,int listenfd)
{
    int clntfd;
    struct sockaddr_in clntAddr;
    socklen_t clntAddrlen;
    clntAddrlen=sizeof(clntAddr);
    clntfd=accept(listenfd,(sockaddr*)&clntAddr,&clntAddrlen);
    if(clntfd==-1)
    {
         printf("accept error!\n");
    }else
    {
        connects++;
        printf("accept a new client: %s counts:%d\n",inet_ntoa(clntAddr.sin_addr),connects);
        add_events(epollfd,clntfd,EPOLLIN);
    }
    do_write(epollfd,clntfd,"ping");
}
 void EpollBase::do_read(int epollfd,int fd,char *buf)
 {
    int nread;
    nread=read(fd,buf,MAXSIZE);
    if(nread==-1)
    {
        printf("read error!\n");
        close(fd);
        delete_events(epollfd,fd,EPOLLIN);
    }else if(nread==0)
    {
        printf("client close\n");
        close(fd);
        delete_events(epollfd,fd,EPOLLIN);
    }
    else
    {
        printf("read message:%s\n",buf);
        modify_events(epollfd,fd,EPOLLOUT);
    }
 }
  void EpollBase::do_write(int epollfd,int fd,char *buf)
 {
    int nwrite;
    nwrite=write(fd,"ping",5);
    if(nwrite==-1)
    {
        printf("write error!\n");
        close(fd);
        delete_events(epollfd,fd,EPOLLIN);
    }
    else
    {
        printf("send message over\n");
        modify_events(epollfd,fd,EPOLLIN);
    }
 }
 void EpollBase::add_events(int epollfd, int fd, int state)
{
	struct epoll_event ev;
	ev.events = state;
	ev.data.fd = fd;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}

void EpollBase::delete_events(int epollfd, int fd, int state)
{
	struct epoll_event ev;
	ev.events = state;
	ev.data.fd = fd;
	epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);
}

void EpollBase::modify_events(int epollfd, int fd, int state)
{
	struct epoll_event ev;
	ev.events = state;
	ev.data.fd = fd;
	epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
}
