#ifndef CLIENTCONNECTOR_H
#define CLIENTCONNECTOR_H

#include "clientinterface.h"
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <QStringList>


class ClientConnector : public ClientInterface
{
public:
    ClientConnector();
    void SendMessage(const std::string Message) override;
    void initiateConnection() override;
    void endConnection() override;
    bool interpretMessage(QStringList message) override;
    int getSocket() const override;
    void setSocket(const int &sock) override;
    QStringList SeparateMessage(std::string message) override;

private:
    int CSocket;
};

#endif // CLIENTCONNECTOR_H
