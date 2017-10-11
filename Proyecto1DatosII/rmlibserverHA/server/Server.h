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
#include "../list/ListHandler.cpp"
#include "../container/InfoContainer.h"
#include "../encryption/encryption.h"

#define BUFFER_SIZE 256

class Server
{

private:
    bool isAlive;
    int socket_desc, client_sock, c, *new_sock;
    struct sockaddr_in server, client;
    ListHandler<InfoContainer> linkedDataBase;
    ListHandler<InfoContainer> cacheDataBase;

public:
    Server();
    void init();
    void run();
    void manage(int *socket_desc);
    void kill();
    void revive();
    void print();

    void sendSwitcher(int *socket_desc, std::vector<std::string> &message);
    const std::vector<std::string> split(const std::string &split, const char &limiter);

    void add(InfoContainer container);
    void erase(std::string key);
    InfoContainer find(std::string key);
};


#endif //SERVER_H