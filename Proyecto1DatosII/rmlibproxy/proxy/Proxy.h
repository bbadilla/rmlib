#ifndef PROXY_H
#define PROXY_H

#include <cstdio>
#include <cstring>    //strlen
#include <cstdlib>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>
#include <iostream>
#include <thread>
#include <vector>

#define BUFFER_SIZE 256

class Proxy
{
private:
    // Server attributes
    int socket_desc, client_sock, c, *new_sock;
    struct sockaddr_in serverProxy, client;

    // Client attributes
    int sock_to_Main, sock_to_HA;
    struct sockaddr_in server_to_Main, server_to_HA;

public:
    void init();
    void initMainServer();
    void initHAServer();
    void run();
    void manage(int* socket_desc);

};


#endif //PROXY_H
