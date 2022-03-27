#include "server.h"
#include "epoll.h"

int main(int argc,char* argv[])
{
    if(argc != 3 && argc != 1)
    {
        std::cout<<"input parament number error"<<std::endl;
    }
    //day01
    // Server s1(argv[1],argv[2]);
    Server s1;
    while(1)
    {
        s1.Server_echo();
    }

    /*未完成
    Epoll epoll;
    epoll.add_fd(s1.get_fd());

    while(1)
    {
        std::vector<epoll_event> ep_events = epoll.ep_wait();

        s1.Server_echo();
    }
    */
    return 0;
}