#include "server.h"

Server::Server(){

    fd = socket(PF_INET,SOCK_STREAM,0);
    assert(fd >= 0);

    struct sockaddr_in server_address;
    bzero(&server_address, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(8888);

    assert(bind(fd,(struct sockaddr*)&server_address,sizeof(server_address)) != -1);
    
    assert(listen(fd,5) != -1);

}

Server::Server(const char* ip,const char* port)
:ip(*ip),port(atoi(port))
{
    fd = socket(PF_INET,SOCK_STREAM,0);
    assert(fd >= 0);

    struct sockaddr_in server_address;
    bzero(&server_address, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(ip);
    server_address.sin_port = htons(this->port);

    assert(bind(fd,(struct sockaddr*)&server_address,sizeof(server_address)) != -1);
    assert(listen(fd,5) != -1);
}

char Server::get_ip()
{
    return this->ip;
}

int Server::get_port()
{
    return this->port;
}

int Server::get_fd()
{
    return this->fd;
}

void Server::Server_echo()
{
    struct sockaddr_in client;
    socklen_t client_addrlen = sizeof(client);

    int connfd = accept(this->fd,(struct sockaddr*)&client, &client_addrlen);
  
    if(connfd < 0)
        std::cout<<"client connect error"<<std::endl; 
    else
    {
        std::cout<<"client fd:"<<connfd<<std::endl;
        int str_len;
        while((str_len = read(connfd,message,BUF_SIZE)) != 0)
        {
            printf("client message:%s",message);
            write(connfd,message,str_len);
        }
        close(connfd);
    }
}

Server::~Server()
{
    close(fd);
}