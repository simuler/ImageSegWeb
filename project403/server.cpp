#include "server.h"

Server::Server(const int& fd)
:fd(fd)
{
    struct sockaddr_in server_address;
    bzero(&server_address, sizeof(server_address));
    this->serv_addr = server_address;
    this->serv_addr_len = sizeof(this->serv_addr);
}

Server::Server(const char* ip,const char* port)
:ip(*ip),port(atoi(port))
{
    fd = socket(PF_INET,SOCK_STREAM,0);
    assert(fd >= 0);

    bzero(&serv_addr, sizeof(serv_addr));
    this->serv_addr_len = sizeof(this->serv_addr);

    this->serv_addr.sin_family = AF_INET;
    this->serv_addr.sin_addr.s_addr = inet_addr(ip);
    this->serv_addr.sin_port = htons(this->port);
}

void Server::server_bind_listen()
{
    assert(::bind(this->fd,(struct sockaddr*)&this->serv_addr,this->serv_addr_len) != -1);
    assert(::listen(this->fd,SOMAXCONN) != -1);
}

void Server::server_connect()
{
    assert(connect(this->fd,(struct sockaddr*)&this->serv_addr,sizeof(this->serv_addr)) != -1);
}

int Server::server_accept(struct sockaddr_in& client_addr)
{
    socklen_t client_addr_len = sizeof(client_addr);  
    int client_fd = accept(this->fd,(struct sockaddr*)&client_addr, &(client_addr_len));
    assert(client_fd != -1);
    return client_fd;
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

void Server::server_echo(const int& connfd)
{
    std::cout<<"client fd:"<<connfd<<std::endl;
    int str_len;
    bzero(&message,sizeof(message));
    while(1)
    {
        str_len = read(connfd,message,BUF_SIZE);
        if(str_len == 0)
        {
            return ;
        }else if(str_len < 0)
        {
            if(errno == EAGAIN)
                return ;
        }else
        {
            printf("message:%s\n",message);
            write(connfd,message,str_len);
        }
    }
}

Server::~Server()
{
    close(fd);
}