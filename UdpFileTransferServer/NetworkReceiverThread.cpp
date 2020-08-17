#include "NetworkReceiverThread.h"
#include <iostream>
#include <fstream>
#include <WS2tcpip.h>
#include <QtDebug>
#include <QBuffer>

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

    //QBuffer buffer(&byteArray);

    cout << "NetworkReceiverThread::run()" << endl;
    WSADATA data;

    // To start WinSock, the required version must be passed to
    // WSAStartup(). This server is going to use WinSock version
    // 2 so I create a word that will store 2 and 2 in hex i.e.
    // 0x0202
    WORD version = MAKEWORD(2, 2);

    // Start WinSock
    int wsOk = WSAStartup(version, &data);
    if (wsOk != 0)
    {
        // Not ok! Get out quickly
        cout << "Can't start Winsock! " << wsOk;
        emit newError("Can't start Winsock!");
        return;
    }

    ////////////////////////////////////////////////////////////
    // SOCKET CREATION AND BINDING
    ////////////////////////////////////////////////////////////

    // Create a socket, notice that it is a user datagram socket (UDP)
    SOCKET in = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // Create a server hint structure for the server
    sockaddr_in serverHint;
    serverHint.sin_addr.S_un.S_addr = ADDR_ANY; // Us any IP address available on the machine
    serverHint.sin_family = AF_INET; // Address format is IPv4
    serverHint.sin_port = htons(54000); // Convert from little to big endian

    // Try and bind the socket to the IP and port
    if (::bind(in, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR)
    {
        cout << "Can't bind socket! " << WSAGetLastError() << endl;
        emit newError("Can't bind socket!");
        return;
    }

    ////////////////////////////////////////////////////////////
    // MAIN LOOP SETUP AND ENTRY
    ////////////////////////////////////////////////////////////

    sockaddr_in client; // Use to hold the client information (port / ip address)
    int clientLength = sizeof(client); // The size of the client information

    //char buffer[1024];
    // Enter a loop
    while (!quit)
    {
        ZeroMemory(&client, clientLength); // Clear the client structure
//        // Wait for message
//        int bytesIn = recvfrom(in, buffer, 1024, 0, (sockaddr*)&client, &clientLength);
//        if (bytesIn == SOCKET_ERROR)
//        {
//            cout << "Error receiving from client " << WSAGetLastError() << endl;
//            continue;
//        }

//        // Display message and client info
//        char clientIp[256]; // Create enough space to convert the address byte array
//        ZeroMemory(clientIp, 256); // to string of characters

//        // Convert from byte array to chars
//        inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);
//        uint16_t port = ntohs(client.sin_port);

//        // Display the message / who sent it
//        cout << "Message recv from " << clientIp << " : " << buffer << endl;
//        string clientIpStr(clientIp);
//        string clientStr = clientIpStr + ":" + to_string(port);
//        emit newMessage(QString::fromStdString(clientStr), buffer);
        QByteArray byteArray;
        const int BUFFER_SIZE = 1024;
        char bufferFile[BUFFER_SIZE];
        int bytesIn;
        long fileDownloaded = 0;
        //std::ofstream file;
        //file.open("newFile.jpg", std::ios::binary | std::ios::trunc);
        long fileRequestedsize = 0;
        recvfrom(in, (char*)&fileRequestedsize, sizeof(long), 0, (sockaddr*)&client, &clientLength);
        while (fileDownloaded < fileRequestedsize) //bytesIn == BUFFER_SIZE);
        {
            memset(bufferFile, 0, BUFFER_SIZE);
            bytesIn = recvfrom(in, bufferFile, BUFFER_SIZE, 0, (sockaddr*)&client, &clientLength);
            cout << "bytesIn: " << bytesIn << endl;
            if (bytesIn == 0 || bytesIn == SOCKET_ERROR) {
                cout << "bytesIn == 0 || bytesIn == -1" << endl;
                break;
            }
            byteArray.append(bufferFile, bytesIn);
            //file.write(bufferFile, bytesIn);
            fileDownloaded += bytesIn;
            cout << "fileSize: " << fileRequestedsize << " package: " << fileDownloaded << endl;
        }
        //file.close();
        //cout << "while (true) ended" << endl;
        char clientIp[256]; // Create enough space to convert the address byte array
        ZeroMemory(clientIp, 256); // to string of characters
        // Convert from byte array to chars
        inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);
        uint16_t port = ntohs(client.sin_port);
        string clientIpStr(clientIp);
        string clientStr = clientIpStr + ":" + to_string(port);
        cout << "Message received from " << clientStr << endl;
        emit newFile(QString::fromStdString(clientStr), byteArray);
    }

    // Close socket
    closesocket(in);

    // Shutdown winsock
    WSACleanup();
}
