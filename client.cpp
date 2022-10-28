#include"client.h"
int count = 0;
void CClient::handle_connection(int sockfd)
{
	int epollfd;
	struct epoll_event events[EPOLLEVENTS];
	char buf[MAXSIZE];
	int ret;
	epollfd = epoll_create(FDSIZE);
	add_event(epollfd, sockfd, EPOLLIN);

	while(1)
	{
		ret = epoll_wait(epollfd, events, EPOLLEVENTS, -1);
		handle_events(epollfd, events, ret, sockfd, buf);
	}
	close(epollfd);
}

void CClient::handle_events(int epollfd, struct epoll_event* events, int num, int sockfd, char* buf)
{
	int fd, i;
	for(i = 0; i<num; i++)
	{
		fd = events[i].data.fd;
		if(events[i].events & EPOLLIN)
			do_read(epollfd, fd, sockfd, buf);
		else if(events[i].events & EPOLLOUT)
			do_write(epollfd, fd, sockfd, buf);
	}
}

void CClient::do_read(int epollfd, int fd, int sockfd, char* buf)
{
	int nread;
	nread = read(fd, buf, MAXSIZE);
	if(nread == -1)
	{
		printf("read error:\n");
		close(fd);
	}
	else if(nread == 0)
	{
		printf("server close.\n");
		close(fd);
	}
	else
	{
        printf("server:%s\n",buf);
        modify_event(epollfd, fd, EPOLLOUT);
        
	}
}

void CClient::do_write(int epollfd, int fd, int sockfd, char* buf)
{
	int nwrite;

	nwrite = write(fd, "pong", 5);
	if(nwrite == -1)
	{
		printf("write error:\n");
		close(fd);
	}
	else
	{
		if(fd == STDOUT_FILENO)
			delete_event(epollfd, fd, EPOLLOUT);
		else
        {
            modify_event(epollfd, fd, EPOLLIN);
            printf("send pong\n");
        }
			
	}
	memset(buf, 0, MAXSIZE);
}

void CClient::add_event(int epollfd, int fd,int state)
{
	struct epoll_event ev;
	ev.events = state;
	ev.data.fd = fd;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}

void CClient::delete_event(int epollfd, int fd, int state)
{
	struct epoll_event ev;
	ev.events = state;
	ev.data.fd = fd;
	epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);
}

void CClient::modify_event(int epollfd, int fd, int state)
{
	struct epoll_event ev;
	ev.events = state;
	ev.data.fd = fd;
	epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
}
