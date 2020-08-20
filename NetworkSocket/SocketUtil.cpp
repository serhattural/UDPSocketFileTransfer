#include "SocketUtil.h"
#include <WS2tcpip.h>
#include <iostream>

bool SocketUtil::Initialize()
{
    WSAData data;
    WORD ver = MAKEWORD(2, 2);

    int wsInit = WSAStartup(ver, &data);
    if (wsInit != 0) //If winsock API failed to start up
    {
        std::cerr << "Failed to start up the winsock API." << std::endl;
    }

    return wsInit == 0;
}

void SocketUtil::Shutdown()
{
    WSACleanup();
}
