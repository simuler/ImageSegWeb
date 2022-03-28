#include "epoll.h"

Epoll::Epoll()
{
    epfd = epoll_create(EPOLL_SIZE);
    assert(epfd != -1);
    events = new epoll_event[EPOLL_SIZE];
    bzero(events,sizeof(*events) * EPOLL_SIZE);
}

Epoll::~Epoll()
{
    close(epfd);
    delete[] events;
}

void Epoll::add_fd(const int& fd)
{
    struct epoll_event event;
    bzero(&event,sizeof(event));
    
    event.data.fd = fd;
    printf("epoll add fd:%d\n", fd);
    event.events = EPOLLIN | EPOLLET; // 边缘触发方式
    epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event);
}

void Epoll::del_fd(const int& fd)
{
    epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
    close(fd);
}

void Epoll::mod_fd(const int& fd){

}

std::vector<epoll_event> Epoll::ep_wait()
{
    std::vector<epoll_event> vec;

    int num = epoll_wait(epfd,events,EPOLL_SIZE,-1);
    assert(num != -1);
    printf("num%d\n",num);
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