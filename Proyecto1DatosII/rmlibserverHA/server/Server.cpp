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
    std::vector<std::string> container;

    while ((read_size = recv(sock, client_message, BUFFER_SIZE, 0)) > 0 && isAlive)
    {
        //std::string msg(decrypt(client_message));
        std::string msg((client_message));
        container = split(msg, ',');

        sendSwitcher(&sock, container);
        std::cout << msg << std::endl;
        print();

        container.clear();
        bzero(client_message, BUFFER_SIZE);
    }

    free(socket_desc);
    close(sock);

    pthread_exit(NULL);
}

void Server::sendSwitcher(int* socket_desc, std::vector<std::string>& message)
{
    int sock = *socket_desc;
    char callBackBuffer[BUFFER_SIZE];


    std::cout << message[2] << std::endl;

    InfoContainer package;
    package.set_key(message[1]);
    package.set_size(atoi(message[2].c_str()));
    //package.set_key(message[3]);

    std::string callBackMessage;

    switch (message[0][0])
    {
        case '1':

            if (linkedDataBase.isInList(message[1]) || cacheDataBase.isInList(message[1]))
            {
                callBackMessage = "EXCEPTION_IS_IN_LIST,1";
            }
            else
            {
                add(package);
                callBackMessage = "Added";
            }
            break;
        case '2':
            if (!linkedDataBase.isInList(message[1]) && !cacheDataBase.isInList(message[1]))
            {
                callBackMessage = "-1,";
            }
            else
            {
                callBackMessage = find(message[1]).to_string();
            }
            break;
        case '3':
            if (!linkedDataBase.isInList(message[1]) && !cacheDataBase.isInList(message[1]))
            {
                callBackMessage = "EXCEPTION_NOT_DELETED,1";
            }
            else
            {
                erase(message[1]);
                callBackMessage = "Removed";
            }
            break;
        default:
            callBackMessage = "Unspecified action";
    }

    //strcpy(callBackBuffer, encrypt(callBackMessage).c_str());
    strcpy(callBackBuffer, callBackMessage.c_str());
    write(sock, callBackBuffer, strlen(callBackBuffer));
}

const std::vector<std::string> Server::split(const std::string& split, const char& limiter)
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

void Server::add(InfoContainer container)
{

    if (cacheDataBase.ssize() < 5)
    {
        cacheDataBase.add(container);
        cacheDataBase.more_ssize();
    }
    else
    {
        InfoContainer temp = cacheDataBase.getPosition(0);
        cacheDataBase.erase(temp.get_key());
        linkedDataBase.add(temp);
        cacheDataBase.add(container);
    }
}

void Server::erase(std::string key)
{
    if (cacheDataBase.isInList(key))
    {
        cacheDataBase.erase(key);
        cacheDataBase.less_ssize();
    }
    else if (linkedDataBase.isInList(key))
    {
        linkedDataBase.erase(key);
    }
    else
    {
        std::cout << "NODO NO ELIMINADO" << std::endl;
    }
}

InfoContainer Server::find(std::string key)
{
    InfoContainer exception;
    exception.set_key("-1");

    if (cacheDataBase.isInList(key))
    {
        return cacheDataBase.find(key);
    }
    else if (linkedDataBase.isInList(key))
    {
        return linkedDataBase.find(key);
    }
    else
    {
        return exception;
    }
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

void Server::print()
{

    std::cout << "/////////////////////////////////////" << std::endl;
    for (int i = 0; i < linkedDataBase.size(); i++)
    {
        std::cout << linkedDataBase[i].get_key() << "," << linkedDataBase[i].get_size() << std::endl;
    }
    std::cout << "////////////////////////////////////////" << std::endl;
    std::cout << "Tamaño: " << linkedDataBase.size() << std::endl;

    std::cout << "////////////////////CACHE///////////////" << std::endl;
    for (int i = 0; i < cacheDataBase.size(); i++)
    {
        std::cout << cacheDataBase[i].get_key() << "," << cacheDataBase[i].get_size() << std::endl;
    }
    std::cout << "////////////////////////////////////////" << std::endl;
    std::cout << "Tamaño: " << cacheDataBase.size() << std::endl;
}
