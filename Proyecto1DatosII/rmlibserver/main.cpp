#include <thread>
#include <iostream>
#include "server/Server.h"

void runServer(Server& server);

int main()
{
    Server server;
    server.init();
    server.initDatabaseSocket();

    std::thread serverThread(runServer, std::ref(server));
    serverThread.detach();

    while (true)
    {
        std::string a = "";
        std::cin >> a;

        if (a == "kill")
        {
            server.kill();
        }
        else if (a == "revive")
        {
            server.revive();
        }
    }


    return 0;
}

void runServer(Server& server)
{
    // The server starts
    server.run();
}
