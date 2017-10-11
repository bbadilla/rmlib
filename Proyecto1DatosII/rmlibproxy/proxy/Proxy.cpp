#include "Proxy.h"

void Proxy::init()
{

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        std::cout << "Could not create socket" << std::endl;
    }
    puts("Socket created");

    serverProxy.sin_family = AF_INET;
    serverProxy.sin_addr.s_addr = INADDR_ANY;
    serverProxy.sin_port = htons( 10269 );

    if( bind(socket_desc,(struct sockaddr *)&serverProxy, sizeof(serverProxy)) < 0)
    {
        perror("bind failed. Error");
        exit(EXIT_FAILURE);
    }

    puts("bind done");

    listen(socket_desc, 3);

    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
}

void Proxy::initMainServer()
{
    sock_to_Main = socket(AF_INET , SOCK_STREAM , 0);
    if (sock_to_Main == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    server_to_Main.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_to_Main.sin_family = AF_INET;
    server_to_Main.sin_port = htons( 10468 );

    //Connect to remote serverProxy
    if (connect(sock_to_Main , (struct sockaddr *)&server_to_Main , sizeof(server_to_Main)) < 0)
    {
        perror("connect failed. Error");
        return;
    }

    puts("Connected\n");
}

void Proxy::initHAServer()
{
    sock_to_HA = socket(AF_INET , SOCK_STREAM , 0);
    if (sock_to_HA == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    server_to_HA.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_to_HA.sin_family = AF_INET;
    server_to_HA.sin_port = htons( 10268 );

    //Connect to remote serverProxy
    if (connect(sock_to_HA , (struct sockaddr *)&server_to_HA , sizeof(server_to_HA)) < 0)
    {
        perror("connect failed. Error");
        return;
    }

    puts("Connected\n");
}

void Proxy::run()
{
    while((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)))
    {
        puts("Connection accepted");

        new_sock = (int*) malloc(sizeof *new_sock);
        *new_sock = client_sock;

        std::thread clientManager(&Proxy::manage, this, std::ref(new_sock));
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

void Proxy::manage(int* socket_desc)
{
    int sock = *socket_desc;
    int read_size;
    char client_message[BUFFER_SIZE];

    while( (read_size = recv(sock, client_message, BUFFER_SIZE, 0)) > 0)
    {
        //std::string msg(decrypt(client_message));
        std::string msg((client_message));

        std::cout << msg << std::endl;

        if (send(sock_to_Main, client_message, strlen(client_message), 0) < 0)
        {
            puts("Send failed");
            return;
        }

        char server_reply[BUFFER_SIZE];
        recv(sock_to_Main, server_reply, BUFFER_SIZE, 0);

        write(sock , server_reply , strlen(server_reply));

        /*if (send(sock_to_HA, client_message, strlen(client_message), 0) < 0)
        {
            puts("Send failed");
            return;
        }

        char server_replyHA[BUFFER_SIZE];
        recv(sock_to_HA, server_replyHA, BUFFER_SIZE, 0);

        write(sock , server_replyHA , strlen(server_replyHA));*/

        bzero(client_message, BUFFER_SIZE);
        bzero(server_reply, BUFFER_SIZE);
        //bzero(server_replyHA, BUFFER_SIZE);
    }

    free(socket_desc);
    close(sock);

    pthread_exit(NULL);
}