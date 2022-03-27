#pragma once
#include <iostream>
#include <sys/socket.h>
#include <assert.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
// #include <stdlib.h>

const int BUF_SIZE = 1024;

class Server{
public:
    Server();
    Server(const char* ,const char* );
    char get_ip();
    int get_port();
    int get_fd();
    void Server_echo();
    ~Server();

private:
    char ip;
    int port;
    int fd;
    char message[BUF_SIZE];
};