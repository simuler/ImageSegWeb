#include "server.h"


int main(int argc, char* argv[]) {
    
    Server s1(argv[1],argv[2]);

    s1.server_connect();
    printf("333\n");
    s1.client_echo();
    printf("444");
    return 0;
}
