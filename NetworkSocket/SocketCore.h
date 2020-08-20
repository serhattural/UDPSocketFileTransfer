#ifndef SOCKETCORE_H
#define SOCKETCORE_H
#include <WinSock2.h>
class SocketCore
{
public:
    SocketCore();
    SOCKET CreateSocket();
    bool CloseSocket();
    bool BindPort(int port);
    bool Recieve(void* destination, int numberOfBytes, int &bytesReceived, sockaddr_in &client);
    bool RecieveAll(void* destination, int numberOfBytes);
    bool Send(const void* data, int numberOfBytes, int &bytesSent, sockaddr_in server);
private:
    SOCKET handle;
};

#endif // SOCKETCORE_H
