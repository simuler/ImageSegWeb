#include "server.h"
#include "epoll.h"
#include "threadpool.h"
#include <pthread.h>
#include <thread>
#include "util.h"
#include "http.h"

void setblocking(const int& fd)
{
    int old_option = fcntl(fd,F_GETFL); 
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd,F_SETFL,new_option);
}

void echo_s(int& connfd)
{
    int str_len;
    std::string message;
    while(1)
    {
        str_len = readn(connfd,message);
        std::cout<<message<<std::endl;
        if(str_len = 0)
        {
            return ;
        }else if(str_len < 0)
        {
            if(errno == EAGAIN)
                return;
        }else
        {
            writen(connfd,message);
        }
    }
}

void http_echo(int& connfd){
    Http ht(connfd);
    ht.http_loop();
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

    pthread_t p_id;
    ThreadPool th(4);

    struct sockaddr_in client_addr;
    while(1)
    {
        std::vector<epoll_event> ep_events = epoll.ep_wait();
        int event_num = ep_events.size();

        for(int i=0;i<event_num;i++)
        {
            if(ep_events[i].data.fd == server->get_fd())
            {

                int client_fd = server->server_accept(client_addr);

                setblocking(client_fd);
                epoll.add_fd(client_fd);     
            }else
            {
                int c_fd = ep_events[i].data.fd;
                // auto lam = [](Server* server,int a){server->server_echo(a);};
                
                //使用pthread.h库运行多线程
                // pthread_create(&p_id,NULL,echo_s,(void*)&c_fd);
                // pthread_detach(p_id);


                // th.add(echo_s,c_fd);
                th.add(http_echo,c_fd);
            }
        }
    }
    return 0;
}