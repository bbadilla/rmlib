#ifndef SERVER_H
#define SERVER_H

#include <cstdio>
#include <cstring>    //strlen
#include <cstdlib>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <pthread.h> //for threading , link with lpthread
#include <iostream>
#include <vector>
#include <thread>
#include "../encryption/encryption.h"

#define BUFFER_SIZE 256

class Server
{

private:

    // Server attributes
    bool isAlive;
    int socket_desc, client_sock, c, *new_sock;
    struct sockaddr_in server, client;

    //Client attributes
    int sock_to_DataBase;
    struct sockaddr_in server_to_DataBase;

public:
    Server();
    void init();
    void initDatabaseSocket();
    void run();
    void manage(int *socket_desc);
    void kill();
    void revive();
};


#endif //SERVER_H