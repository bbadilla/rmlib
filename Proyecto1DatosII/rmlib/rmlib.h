#ifndef RMLIB_H
#define RMLIB_H

#include<cstdio> //printf
#include<cstring>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include <unistd.h>  // close socket
#include <iostream>
#include <vector>
#include "rmref_h/rmRef_H.h"
#include "encryption/encryption.h"

#define BUFFER_SIZE 256

void rm_init (char* ip, int port, char* ipHA, int portHA);

void rm_new (char* _key, void* value, int value_size);

rmRef_H rm_get(char* _key);

void rm_delete(rmRef_H* handler);

std::vector<std::string> split(const std::string& split, const char& limiter);

std::string serialize(void* ptr, int size);

void* deserialize(std::string encoded);

#endif
