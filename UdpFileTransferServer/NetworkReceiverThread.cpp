#include "NetworkReceiverThread.h"
#include <iostream>
#include <fstream>
#include <WS2tcpip.h>
#include <QtDebug>
#include <QBuffer>
#include "SocketUtil.h"
#include "SocketCore.h"

using namespace std;

NetworkReceiverThread::NetworkReceiverThread(QObject *parent)
    : QThread(parent), quit(false)
{
}

NetworkReceiverThread::~NetworkReceiverThread()
{
    mutex.lock();
    quit = true;
    mutex.unlock();
    wait();
}

void NetworkReceiverThread::run()
{
    SocketUtil::Initialize();
    SocketCore socketCore;

    SOCKET socketIn = socketCore.CreateSocket();
    if (socketIn == INVALID_SOCKET)
    {
        cout << "Invalid socket on create socket." << endl;
        return;
    }

    if (!socketCore.BindPort(this->port))
    {
        cout << "Can't bind socket" << endl;
        return;
    }

    sockaddr_in client; // Use to hold the client information (port / ip address)
    long fileSize = 0;
    int receivedBytes = 0;
    while (!quit)
    {
        socketCore.Recieve(&fileSize, sizeof(fileSize), receivedBytes, client);
        char* fileByteArray = new char[fileSize];
        socketCore.RecieveAll(fileByteArray, fileSize);

        string clientStr = ConvertAddressToString(client);

        emit newFile(QString::fromStdString(clientStr), QByteArray(fileByteArray, fileSize));
        delete[] fileByteArray;
    }

    // Close socket
    socketCore.CloseSocket();
    SocketUtil::Shutdown();
}

string NetworkReceiverThread::ConvertAddressToString(sockaddr_in client)
{
    char clientIp[256];
    ZeroMemory(clientIp, 256);
    inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);
    uint16_t port = ntohs(client.sin_port);
    string clientIpStr(clientIp);
    string clientStr = clientIpStr + ":" + to_string(port);
    return clientStr;
}
