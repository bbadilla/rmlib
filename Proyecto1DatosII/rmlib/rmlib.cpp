#include <sstream>
#include "rmlib.h"

int sock;
struct sockaddr_in server;

void rm_init (char* ip, int port, char* ipHA, int portHA)
{
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons( port );

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return;
    }

    puts("Connected\n");

}

void rm_new (char* _key, void* value, int value_size)
{
    try
    {
        std::string msg = "1," + std::string(_key) + "," + std::to_string(value_size) + "," +
                                                                serialize(value, value_size);

        char *key = (char *) encrypt(msg).c_str();
        if (send(sock, key, strlen(key), 0) < 0)
        {
            puts("Send failed");
            return;
        }

        char server_reply[BUFFER_SIZE];
        recv(sock, server_reply, BUFFER_SIZE, 0);
        std::string reply(server_reply);

        if (split(decrypt(server_reply), ',')[0] == "-1")
            throw 1;

        std::cout << decrypt(reply) << std::endl;
        //std::cout << reply << std::endl;
    }
    catch (int e)
    {
        std::cout << "The node was already on the list. ERROR: " << e << std::endl;
    }
}

rmRef_H rm_get(char* _key)
{
    rmRef_H ref_h;

    std::string msg1 = "2," + std::string(_key) + ",";
    char* key = (char*) encrypt(msg1).c_str();;

    if( send(sock, key, strlen(key) , 0) <= 0)
    {
        puts("Send failed");
    }

    char server_reply[BUFFER_SIZE];
    recv(sock , server_reply , BUFFER_SIZE , 0);

    std::string msg(server_reply);
    std::vector<std::string> splitMessage = split(decrypt(msg), ',');
    //std::vector<std::string> splitMessage = split((msg), ',');

    std::cout << "VALUE: " << splitMessage[2] << std::endl;

    void* temp = deserialize(splitMessage[2]);
   // std::cout<<"deserialize: "<<temp<<std::endl;;

    ref_h.set_key(splitMessage[0]);
    ref_h.set_size(atoi(splitMessage[1].c_str()));
    ref_h.set_value(temp);
    ref_h.set_svalue(splitMessage[2]);

    if (split(decrypt(ref_h.get_key()), ',')[0] == "-1")
        ref_h.set_key("-1");

    return ref_h;
}

void rm_delete(rmRef_H* handler)
{
    try
    {
        std::string msg = "3," + handler->get_key() + ",";
        char *key = (char *) encrypt(msg).c_str();

        if (send(sock, key, strlen(key), 0) < 0) {
            puts("Send failed");
        }

        char server_reply[BUFFER_SIZE];
        recv(sock, server_reply, 2000, 0);
        std::string reply(server_reply);

        if (split(decrypt(server_reply), ',')[0] == "-1")
            throw 3;

        std::cout << decrypt(server_reply) << std::endl;
    }
    catch (int e)
    {
        std::cout << "The node was not deleted. ERROR: " << e << std::endl;
    }

}

std::vector<std::string> split(const std::string& split, const char& limiter)
{
    std::string buff;
    std::vector<std::string> resultVector;

    for(auto i : split)
    {
        if(i != limiter)
            buff += i;
        else if(i == limiter && buff != "")
        {
            resultVector.push_back(buff);
            buff = "";
        }
    }

    if(buff != "")
        resultVector.push_back(buff);

    return resultVector;
}

std::string serialize(void* ptr, int size)
{
    std::string final;

    char* data = (char*) ptr;

    for (int i = 0; i < size; ++i)
    {
        final += data[i];
    }

    return final;
}

void* deserialize(std::string encoded)
{
    char* data = (char*) encoded.c_str();

    return (void*) data;
}




