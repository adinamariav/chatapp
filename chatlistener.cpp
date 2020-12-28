#include "chatlistener.h"
#include "mainwindow.h"

ChatListener* ChatListener::instance =  nullptr;

ChatListener::ChatListener(const int& socket, QObject *parent) :
    QObject(parent)
{
   Poll.fd = socket;
   Poll.events = POLLIN;
}


ChatListener *ChatListener::getInstance(const int &socket)
{
    if(instance == nullptr)
    {
        instance = new ChatListener(socket);
    }

    return instance;
}

void ChatListener::process()
{
    while(1)
    {
        auto mess = this->Listen();
        if(mess != "")
        {
           emit emitSignal(mess);
        }
    }
}

void ChatListener::finish()
{
    emit finished();
}

QString ChatListener::Listen()
{
    char Buffer[1024]; //pentru primirea raspunsului

    while(1)
    {
        poll(&Poll, 1, 5000);
        if(Poll.revents & POLLIN)
        {
            Poll.revents = 0;
            memset(Buffer, 0, 1024);
            read(Poll.fd, Buffer, 1024);
            std::string Answer = std::string(Buffer);
            return QString::fromStdString(Answer);
        }
    }
}

QStringList ChatListener::SeparateMessage(QString message)
{
    QStringList messagePieces = message.split("`");

    return messagePieces;
}


void ChatListener::emitSignal(const QString &answer)
{
    auto messPieces = this->SeparateMessage(answer);
    auto command = messPieces.front().toStdString();

    if(command == Constants::Login)
        emit receiveLogin(answer);
    if(command == Constants::Logout)
        emit receiveLogout(answer);
    if(command == Constants::SignUp)
        emit receiveSignup(answer);
    if(command == Constants::ChangeUsername)
        emit receiveChangeU(answer);
    if(command == Constants::ChangePassword)
        emit receiveChangeP(answer);
    if(command == Constants::InitUsers)
        emit receiveInitU(answer);
    if(command == Constants::InitWindow)
        emit receiveInitW(answer);
    if(command == Constants::SearchUsers)
        emit receiveSearch(answer);
    if(command == Constants::SendMessage)
        emit receiveSendM(answer);
    if(command == Constants::ReceiveMessage)
        emit receiveMess(answer);
}

void ChatListener::setupConnections()
{
   //QObject::connect(this, SIGNAL(emitSignal(QString)), MainWindow, SLOT(process()));
}

