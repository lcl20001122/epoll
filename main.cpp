#include"server.h"

int main()
{
    int listenfd;
    EpollBase *ep=new EpollBase;
    listenfd=ep->socket_bind("192.168.92.132",8080);
    listen(listenfd,1);
    ep->do_epoll(listenfd);
    return 0;
}
