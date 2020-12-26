#ifndef CLIENTINTERFACE_H
#define CLIENTINTERFACE_H


#include <string>
#include <QList>
#include <QString>
#include <QStringList>

class ClientInterface
{
public:
    ClientInterface();

    virtual std::string SendMessage(const std::string message) = 0;
    virtual std::string Listen() = 0;
    virtual void SendMessageVoid(const std::string message) = 0;
    virtual void initiateConnection() = 0;
    virtual void endConnection() = 0;
    virtual QStringList SeparateMessage(std::string message) = 0;
    virtual bool interpretMessage(QStringList message) = 0;
    virtual int getSocket() const = 0;
    virtual void setSocket(const int& sock) = 0;
};

#endif // CLIENTINTERFACE_H
