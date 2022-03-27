#include "epoll.h"

Epoll::Epoll()
{
    this->epfd = epoll_create(0);
    this->events = new epoll_event[MAX_EVENT_NUM];
}

Epoll::~Epoll()
{
    close(epfd);
}

void Epoll::add_fd(const int& fd)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET; // 边缘触发方式
    epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event);

}

void Epoll::del_fd(const int& fd)
{
    epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
}

void Epoll::mod_fd(const int& fd){

}

std::vector<epoll_event> Epoll::ep_wait()
{
    std::vector<epoll_event> vec;
    int num = epoll_wait(epfd,events,EPOLL_SIZE,-1);

    for(int i=0;i<num;i++)
    {
        vec.push_back(events[i]);
    }
    return vec;
}

int Epoll::get_epfd()
{
    return epfd;
}