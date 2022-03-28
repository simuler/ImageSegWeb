#pragma once
#include <iostream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <vector>

const int MAX_EVENT_NUM = 512;
const int EPOLL_SIZE = 50;

class Epoll
{
public:
    Epoll();
    ~Epoll();
    void add_fd(const int&);
    void del_fd(const int&);
    void mod_fd(const int&);
    std::vector<epoll_event> ep_wait();
    int get_epfd();

private:
    int epfd;
    struct epoll_event* events;
};