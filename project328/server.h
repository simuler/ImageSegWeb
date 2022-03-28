#pragma once
#include <iostream>
#include <sys/socket.h>
#include <assert.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>


const int BUF_SIZE = 1024;

class Server{
public:
    Server(const int&);
    Server(const char* ,const char* );
    void server_connect();
    void server_bind_listen();
    int server_accept(struct sockaddr_in& );
    char get_ip();
    int get_port();
    int get_fd();
    int server_echo(const int& );
    void client_echo();
    ~Server();



private:
    struct sockaddr_in serv_addr;
    socklen_t serv_addr_len;
    char ip;
    int port;
    int fd;

    char message[BUF_SIZE];
};