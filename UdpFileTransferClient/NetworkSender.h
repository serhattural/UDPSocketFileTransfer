#ifndef NETWORKSENDER_H
#define NETWORKSENDER_H

#include <iostream>

class NetworkSender
{
public:
    NetworkSender();
    void SendMessageStr(const std::string &message);
    void SendFile(const std::string &fileName);
};

#endif // NETWORKSENDER_H
