#include "chatconversationhandler.h"
#include "chatlistener.h"

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
    interface->SendMessage(MessageGenerator::sendMessage(this->usernameFrom, usernameTo, message));

}

void ChatConversationHandler::sendMessageReceived(QString message)
{
    auto processedAns = interface->SeparateMessage(message.toStdString());

    if(interface->interpretMessage(processedAns) == false)
    {
        //throw exception
    }
}

void ChatConversationHandler::receive(QString message)
{
    emit receiveMess(message);
}

void ChatConversationHandler::InitMessages()
{
    interface->SendMessage(MessageGenerator::initWindow(this->usernameFrom, this->usernameTo));
}

void ChatConversationHandler::initMessagesReceived(QString message)
{
    auto messagePieces = interface->SeparateMessage(message.toStdString());

    emit initMessages(messagePieces);
}

void ChatConversationHandler::setupConnections()
{
    auto listener = ChatListener::getInstance(interface->getSocket());

    connect(listener, SIGNAL(receiveMess(QString)), this, SLOT(receive(QString)));
    connect(listener, SIGNAL(receiveSendM(QString)), this, SLOT(sendMessageReceived(QString)));
    connect(listener, SIGNAL(receiveInitW(QString)), this, SLOT(initMessagesReceived(QString)));
}

