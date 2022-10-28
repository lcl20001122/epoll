#include"client.h"
int main(int argc, char **argv)
{
    CClient *c =new CClient;
    int sockfd=socket(AF_INET,SOCK_STREAM,0);;
    struct sockaddr_in clntAddr;
    bzero(&clntAddr, sizeof(clntAddr));
	clntAddr.sin_family = AF_INET;
    clntAddr.sin_port =0;
    inet_pton(AF_INET, CLIENTIP, &clntAddr.sin_addr);
    if(bind(sockfd,(sockaddr *)&clntAddr,sizeof(clntAddr))==-1)
    {
        printf("bind error!\n");
        exit(1);
    }else
    {
        printf("bind sucess\n");
    }

    
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET, IPADDRESS, &servaddr.sin_addr);
	connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	c->handle_connection(sockfd);
	close(sockfd);
	return 0;
}
