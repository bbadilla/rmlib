#include <thread>
#include <iostream>
#include "server/Server.h"
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <iostream>
#include <glib.h>
#include <fstream>

#include <string>
#include <cstring>

void runServer(Server& server);

int main(int argc, char *argv[])
{
    Server server;
    server.init();

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
