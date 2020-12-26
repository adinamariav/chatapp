#include "chatconversationhandler.h"

ChatConversationHandler::ChatConversationHandler(const QString& usernameFrom, const QString& usernameTo, int socket, QObject *parent) :
    QObject(parent),
    usernameFrom(usernameFrom),
    usernameTo(usernameTo)
{
    interface->setSocket(socket);
}

const QString &ChatConversationHandler::getUsernameTo()
{
    return this->usernameTo;
}

void ChatConversationHandler::process()
{


}

void ChatConversationHandler::finish()
{
    emit finished();
}

void ChatConversationHandler::sendMessage(QString message)
{
    auto ans = interface->SendMessage(MessageGenerator::sendMessage(this->usernameFrom, usernameTo, message));
    auto processedAns = interface->SeparateMessage(ans);

    if(interface->interpretMessage(processedAns) == false)
    {
        //throw exception
    }
}

