#ifndef NETWORKRECEIVERTHREAD_H
#define NETWORKRECEIVERTHREAD_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <WS2tcpip.h>
#include <iostream>

class NetworkReceiverThread : public QThread
{
    Q_OBJECT

public:
    NetworkReceiverThread(QObject *parent = nullptr);
    ~NetworkReceiverThread();

    void run() override;

signals:
    void newMessage(const QString &clientIp, const QString &message);
    void newFile(const QString &clientIp, const QByteArray &buffer);
    void newError(const QString &message);

private:
    std::string ConvertAddressToString(sockaddr_in client);
    QMutex mutex;
    bool quit;
    int port = 54000;
};

#endif // NETWORKRECEIVERTHREAD_H
