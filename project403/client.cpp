#include "server.h"
#include "util.h"

void client_echo(const int& fd)
{
    while(true){
        std::string mess("GET / HTTP/1.0 \r\nUser-Agent:WebBench 1.5\r\nHost:localhost\r\n\r\n");
        std::cout<<mess<<std::endl;

        ssize_t write_bytes = writen(fd,mess);
        if(write_bytes == -1)
        {
            printf("socket already disconnected, can't write any more!\n");
            break;
        }

        std::string me;
     
        ssize_t read_bytes = readn(fd,me);
        if(read_bytes > 0)
        {
            std::cout<<"message from server:"<<me<<std::endl;
        }else if(read_bytes == 0)
        {
            printf("server socket disconnected!\n");
            break;
        }else if(read_bytes < 0)
        {
            printf("send error");
        }
        break;
    }
}

int main(int argc, char* argv[]) {
    
    Server s1(argv[1],argv[2]);

    s1.server_connect();

    client_echo(s1.get_fd());

    return 0;
}
