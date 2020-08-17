#include "NetworkSender.h"
#include <iostream>
#include <fstream>
#include <WS2tcpip.h>
#include <chrono>
#include <thread>

using namespace std;

NetworkSender::NetworkSender()
{

}


void NetworkSender::SendMessageStr(const string &message)
{
    WSADATA data;
    WORD version = MAKEWORD(2, 2);

    // Start WinSock
    int wsOk = WSAStartup(version, &data);
    if (wsOk != 0)
    {
        // Not ok! Get out quickly
        cout << "Can't start Winsock! " << wsOk;
        return;
    }
    sockaddr_in server;
    server.sin_family = AF_INET; // AF_INET = IPv4 addresses
    server.sin_port = htons(54000); // Little to big endian conversion
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr); // Convert from string to byte array

    // Socket creation, note that the socket type is datagram
    SOCKET socketOut = socket(AF_INET, SOCK_DGRAM, 0);

    // Write out to socket
    int sendOk = sendto(socketOut, message.c_str(), message.size() + 1, 0, (sockaddr*)&server, sizeof(server));

    if (sendOk == SOCKET_ERROR)
    {
        cout << "That didn't work! " << WSAGetLastError() << endl;
    }

    // Close the socket
    closesocket(socketOut);

    // Close down Winsock
    WSACleanup();
}

void NetworkSender::SendFile(const string &fileName)
{
    WSADATA data;
    WORD version = MAKEWORD(2, 2);

    // Start WinSock
    int wsOk = WSAStartup(version, &data);
    if (wsOk != 0)
    {
        // Not ok! Get out quickly
        cout << "Can't start Winsock! " << wsOk;
        return;
    }
    sockaddr_in server;
    server.sin_family = AF_INET; // AF_INET = IPv4 addresses
    server.sin_port = htons(54000); // Little to big endian conversion
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr); // Convert from string to byte array

    // Socket creation, note that the socket type is datagram
    SOCKET socketOut = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    const int BUFFER_SIZE = 1024;
    char bufferFile[BUFFER_SIZE];
    int sendOk;
    ifstream file;
    file.open(fileName, std::ios::binary);

    if (file.is_open()) {
        // get file size
        file.seekg(0, std::ios::end);
        long fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        sendto(socketOut, (char*)&fileSize, sizeof(long), 0, (sockaddr*)&server, sizeof(server));
        // read file with do-while loop
        long fileDownloaded = 0;
        do {
            // read and send part file to client
            file.read(bufferFile, BUFFER_SIZE);
            if(file.gcount() > 0)
                sendOk = sendto(socketOut, bufferFile, file.gcount(), 0, (sockaddr*)&server, sizeof(server));

            if (sendOk == 0 || sendOk == -1) {
                // error sending data - break loop
                break;
            }
            fileDownloaded += sendOk;
            cout << "fileSize:" << fileSize << " package: " << fileDownloaded << endl;

            std::this_thread::sleep_for(std::chrono::microseconds(2));
        } while (file.gcount() > 0);
        file.close();

        cout << "Client sent file succesfully" << endl;
    }

}
