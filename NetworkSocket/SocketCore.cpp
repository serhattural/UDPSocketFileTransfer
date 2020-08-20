#include "SocketCore.h"
#include "Constants.h"
#include <iostream>

using namespace std;

SocketCore::SocketCore()
{
    this->handle = INVALID_SOCKET;
}

bool SocketCore::CloseSocket()
{
    if (handle == INVALID_SOCKET)
    {
        return false;
    }

    int result = closesocket(handle);
    if (result != 0) //if error occurred while trying to close socket
    {
        int error = WSAGetLastError();
        return false;
    }

    handle = INVALID_SOCKET;
    return true;
}

SOCKET SocketCore::CreateSocket()
{
    SOCKET handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (handle != INVALID_SOCKET)
    {
        setsockopt(handle, SOL_SOCKET, SO_RCVBUF, (char*)&SENT_RCV_BUFFER_SIZE, sizeof(SENT_RCV_BUFFER_SIZE));
        this->handle = handle;
    }

    return handle;
}

bool SocketCore::BindPort(int port)
{
    if(this->handle != INVALID_SOCKET)
    {
        // Create a server hint structure for the server
        sockaddr_in serverHint;
        serverHint.sin_addr.S_un.S_addr = ADDR_ANY; // Us any IP address available on the machine
        serverHint.sin_family = AF_INET; // Address format is IPv4
        serverHint.sin_port = htons(port); // Convert from little to big endian
        int bindResult = bind(this->handle, (sockaddr*)&serverHint, sizeof(serverHint));
        if (bindResult == SOCKET_ERROR)
        {
            cerr << "Can't bind socket." << WSAGetLastError() << endl;
            return false;
        }
    }
    else
    {
        cerr << "Socket has value INVALID_SOCKET." << WSAGetLastError() << endl;
        return false;
    }
    return true;
}

bool SocketCore::Recieve(void* destination, int numberOfBytes, int &bytesReceived, sockaddr_in &client)
{
    int clientLength = sizeof(sockaddr_in);
    bytesReceived = recvfrom(this->handle, (char*)destination, numberOfBytes, 0, (sockaddr*)&client, &clientLength);

    if (bytesReceived == 0)
    {
        return false;
    }

    if (bytesReceived == SOCKET_ERROR)
    {
        cerr << "Socket error on Receive" << endl;
        return false;
    }

    return true;
}

bool SocketCore::RecieveAll(void* destination, int numberOfBytes)
{
    int totalBytesReceived = 0;
    sockaddr_in client;
    while (totalBytesReceived < numberOfBytes)
    {
        int bytesRemaining = numberOfBytes - totalBytesReceived;
        int bytesReceived = 0;
        char* bufferOffset = (char*)destination + totalBytesReceived;
        bool result = Recieve(bufferOffset, bytesRemaining, bytesReceived, client);
        if (!result)
        {
            cerr << "Error on RecieveAll" << endl;
            return false;
        }
        totalBytesReceived += bytesReceived;
        cout << "fileSize: " << numberOfBytes << " package: " << totalBytesReceived << endl;
    }

    return true;
}

bool SocketCore::Send(const void* data, int numberOfBytes, int &bytesSent, sockaddr_in server)
{
    bytesSent = sendto(this->handle, (const char*)data, numberOfBytes, 0, (sockaddr*)&server, sizeof(server));
    if (bytesSent == SOCKET_ERROR)
    {
        int error = WSAGetLastError();
        cerr << "Error on Send:" << error << endl;
        return false;
    }

    return true;
}
