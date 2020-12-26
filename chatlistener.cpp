#include "chatlistener.h"

ChatListener::ChatListener(const QString& usernameFrom, const QString& usernameTo, const int& socket, QObject *parent) :
    QObject(parent),
    usernameFrom(usernameFrom),
    usernameTo(usernameTo)
{
    interface->setSocket(socket);
}


void ChatListener::process()
{
    while(1)
    {
        auto mess = interface->Listen();
        if(mess != "")
        {
            auto messPieces = interface->SeparateMessage(mess);
            if(messPieces.front() == "receive")
            {
                auto piece = messPieces.begin();
                piece++;
                if(*piece == this->usernameFrom)
                {
                    emit receive(QString::fromStdString(mess));
                }
            }
        }
    }
}

void ChatListener::finish()
{
    emit finished();
}

