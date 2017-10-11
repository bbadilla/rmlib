#include "Server.h"


Server::Server()
{
    //linkedDataBase = new ListHandler<InfoContainer>();
}

void Server::init()
{
    isAlive = true;

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        std::cout << "Could not create socket" << std::endl;
    }
    puts("Socket created");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 10468 );

    if( bind(socket_desc,(struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("bind failed. Error");
        exit(EXIT_FAILURE);
    }

    puts("bind done");

    listen(socket_desc, 3);

    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
}

void Server::initDatabaseSocket()
{
    sock_to_DataBase = socket(AF_INET , SOCK_STREAM , 0);
    if (sock_to_DataBase == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    server_to_DataBase.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_to_DataBase.sin_family = AF_INET;
    server_to_DataBase.sin_port = htons( 10568 );

    //Connect to remote serverProxy
    if (connect(sock_to_DataBase , (struct sockaddr *)&server_to_DataBase , sizeof(server_to_DataBase)) < 0)
    {
        perror("connect failed. Error");
        return;
    }

    puts("Connected\n");
}

void Server::run()
{
    while((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)))
    {
        puts("Connection accepted");

        new_sock = (int*) malloc(sizeof *new_sock);
        *new_sock = client_sock;

        std::thread clientManager(&Server::manage, this, std::ref(new_sock));
        clientManager.detach();
        puts("Handler assigned");
    }

    if (client_sock < 0)
    {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Thread finished" << std::endl;
}

void Server::manage(int* socket_desc) {
    int sock = *socket_desc;
    int read_size;
    char client_message[BUFFER_SIZE];

    while ((read_size = recv(sock, client_message, BUFFER_SIZE, 0)) > 0 && isAlive)
    {
        std::string msg(decrypt(client_message));
        //std::string msg((client_message));
        std::cout << msg << std::endl;

        if (send(sock_to_DataBase, client_message, strlen(client_message), 0) < 0)
        {
            puts("Send failed");
            return;
        }

        char server_reply[BUFFER_SIZE];
        recv(sock_to_DataBase, server_reply, BUFFER_SIZE, 0);

        write(sock , server_reply , strlen(server_reply));

        bzero(client_message, BUFFER_SIZE);
        bzero(server_reply, BUFFER_SIZE);
    }

    free(socket_desc);
    close(sock);

    pthread_exit(NULL);
}

void Server::kill()
{
    isAlive = false;
    std::cout << "KILL" << std::endl;
}

void Server::revive()
{
    isAlive = true;
}


