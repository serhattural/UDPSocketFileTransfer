#include "NetworkSender.h"
#include <iostream>
#include <fstream>
#include <WS2tcpip.h>
#include <chrono>
#include <thread>
#include "SocketUtil.h"
#include "SocketCore.h"
#include "Constants.h"
using namespace std;

NetworkSender::NetworkSender()
{

}

void NetworkSender::SendFile(const string &fileName)
{
    SocketUtil::Initialize();
    SocketCore socketCore;
    SOCKET socketOut = socketCore.CreateSocket();

    sockaddr_in server;
    server.sin_family = AF_INET; // AF_INET = IPv4 addresses
    server.sin_port = htons(54000); // Little to big endian conversion
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr); // Convert from string to byte array

    char bufferFile[FILEDESC_BUFFER_SIZE];
    int sendOk;
    ifstream file;
    file.open(fileName, std::ios::binary);

    if (file.is_open()) {
        // get file size
        file.seekg(0, std::ios::end);
        long fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        socketCore.Send((char*)&fileSize, sizeof(long), sendOk, server);

        long fileDownloaded = 0;
        do {
            // read and send part file to client
            file.read(bufferFile, FILEDESC_BUFFER_SIZE);
            if(file.gcount() > 0){
                sendOk = socketCore.Send(bufferFile, file.gcount(), sendOk, server);
            }
            if (sendOk == 0 || sendOk == -1) {
                break;
            }
            fileDownloaded += sendOk;
            cout << "fileSize:" << fileSize << " package: " << fileDownloaded << endl;
            std::this_thread::sleep_for(std::chrono::microseconds(2));
        } while (file.gcount() > 0);

        file.close();

        cout << "Client has sent file succesfully" << endl;
    }

}
