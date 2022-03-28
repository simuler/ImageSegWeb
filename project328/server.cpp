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
    printf("connect_fd:%d\n",this->fd);
}

int Server::server_accept(struct sockaddr_in& client_addr)
{
    socklen_t client_addr_len = sizeof(client_addr);  
    int client_fd = accept(this->fd,(struct sockaddr*)&client_addr, &(client_addr_len));
    assert(client_fd != -1);
    printf("connect success client_fd:%d\n",client_fd);
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

int Server::server_echo(const int& connfd)
{
    std::cout<<"client fd:"<<connfd<<std::endl;
    int str_len;
    bzero(&message,sizeof(message));
    while(1)
    {
        str_len = read(connfd,message,BUF_SIZE);
        if(str_len == 0)
        {
            return 0;
        }else if(str_len < 0)
        {
            if(errno == EAGAIN)
                return 1;
        }else
        {
            printf("message:%s\n",message);
            write(connfd,message,str_len);
        }
    }
}

void Server::client_echo()
{
    while(true){
        printf("Input message(q to quit):");
        scanf("%s", message);
        if(!strcmp(message,"q"))
            break;

        ssize_t write_bytes = write(fd, message, sizeof(message));
       
        if(write_bytes == -1)
        {
            printf("socket already disconnected, can't write any more!\n");
            break;
        }
        bzero(&message, sizeof(message));
        ssize_t read_bytes = read(fd, message, sizeof(message));
        if(read_bytes > 0)
        {
            printf("message from server: %s\n", message);
        }else if(read_bytes == 0)
        {
            printf("server socket disconnected!\n");
            break;
        }else if(read_bytes < 0)
        {
            printf("send error");
        }
    }
}

Server::~Server()
{
    close(fd);
}