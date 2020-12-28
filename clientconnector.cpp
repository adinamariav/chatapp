#include "clientconnector.h"

ClientConnector::ClientConnector()
{



}

void ClientConnector::initiateConnection()
{
    struct sockaddr_in sockcl;
    struct sockaddr_in to;

    // creare obiect de tip socket pentru client
    if ((this->CSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("client: socket");
        _exit(1);
    }

    sockcl.sin_family=AF_INET;
    sockcl.sin_addr.s_addr=inet_addr("25.95.250.217");

    to.sin_family=AF_INET;
    to.sin_port = htons(64000);
    to.sin_addr.s_addr = inet_addr("25.93.86.205");


    if (bind(this->CSocket, (struct sockaddr *)&sockcl, sizeof(sockcl)) < 0)
    {
        perror("client: bind");
        _exit(1);
    }

    if (connect(this->CSocket,(struct sockaddr *)&to, sizeof(to)) < 0)
    {
        perror("client: connect");
        _exit(1);
    }

    printf("Conectare cu succes\n");
}

void ClientConnector::SendMessage(const std::string Message)
{
    auto mes = Message.c_str();
    auto size = Message.size();

    write(this->CSocket, mes, size + 1);
    sleep(2);
}



void ClientConnector::endConnection()
{
    shutdown(this->CSocket,2);
    close(this->CSocket);
    _exit(0);
}


bool ClientConnector::interpretMessage(QStringList message)
{
    if(message.back() == "no")
            return false;
    else
        if(message.back() == "yes")
            return true;
    else
        if(message.back() == "firsttime")
            return true;

    return false;
}

int ClientConnector::getSocket() const
{
    return this->CSocket;
}

void ClientConnector::setSocket(const int &sock)
{
    this->CSocket = sock;
}

QStringList ClientConnector::SeparateMessage(std::string message)
{
    QString mes = QString::fromStdString(message);
    QStringList messagePieces = mes.split("`");

    return messagePieces;
}
