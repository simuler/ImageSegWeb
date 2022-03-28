#include "server.h"
#include "epoll.h"

void setblocking(const int& fd)
{
    int old_option = fcntl(fd,F_GETFL); 
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd,F_SETFL,new_option);
}

int main(int argc,char* argv[])
{
    if(argc != 3 && argc != 1)
    {
        std::cout<<"input parament number error"<<std::endl;
    }

    Server* server = new Server(argv[1],argv[2]);


    server->server_bind_listen();
    setblocking(server->get_fd());

    Epoll epoll;
    epoll.add_fd(server->get_fd());


    struct sockaddr_in client_addr;
    while(1)
    {
        std::vector<epoll_event> ep_events = epoll.ep_wait();
        int event_num = ep_events.size();
        if(event_num > 0)
            printf("event_num:%d\n",event_num);
        for(int i=0;i<event_num;i++)
        {
            if(ep_events[i].data.fd == server->get_fd())
            {

                int client_fd = server->server_accept(client_addr);

                setblocking(client_fd);
                epoll.add_fd(client_fd);     
                printf("connected client:%d\n",client_fd);
            }else
            {
                if(server->server_echo(ep_events[i].data.fd) == 0)
                    epoll.del_fd(ep_events[i].data.fd);
            }
        }
    }

    return 0;
}