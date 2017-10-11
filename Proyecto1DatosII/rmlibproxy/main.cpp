#include <iostream>
#include <thread>
#include "proxy/Proxy.h"

void runServer(Proxy& proxy);

int main() {

    Proxy proxy;
    proxy.init();
    proxy.initMainServer();
    proxy.initHAServer();

    std::thread serverThread(runServer, std::ref(proxy));
    serverThread.join();

    return 0;
}

void runServer(Proxy& proxy)
{
    // The proxy starts
    proxy.run();
}