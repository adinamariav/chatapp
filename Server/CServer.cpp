#include "CServer.hpp"
#include <stdio.h> 
#include <stdlib.h> //pt exit
#include <string.h>
#include <unistd.h> //read si write, exit
#include <sys/types.h> //contine definitiile diferitor tipuri de date folosite in apelurile de sistem
#include <sys/socket.h> //contine definitiile structurilor necesare pentru socket-uri
#include <netinet/in.h> //contine constante si structuri necesare pentru adrese internet
#include <arpa/inet.h> //inet_addr, inet_ntoa
#include <pthread.h>

CServer* CServer::instance = NULL;

CServer* CServer::getInstance(string IPaddress, int portNumber){
    if(!CServer::instance){
        CServer::instance=new CServer(IPaddress, portNumber);
    }
    return CServer::instance;
}

void CServer::destroyInstance(){
    if(CServer::instance){
        delete CServer::instance;
        CServer::instance = NULL;
    }
}

CServer::CServer(string IPaddress, int portNumber){
    serverSocket=new CSocket(IPaddress, portNumber);
}

CServer::~CServer(){
    delete serverSocket;
    serverSocket=NULL;
    list<CSocket*>::iterator it;
    for(it=clientSocketList.begin(); it!=clientSocketList.end();++it){
        delete *it;
        *it=NULL;
    }
    clientSocketList.clear();
}

CSocket* CServer::getServerSocket(){
    return serverSocket;
}

void CServer::listenForConnections(int maxNumOfConnections){
    listen(serverSocket->getSocketDescriptor(), maxNumOfConnections); //5 is the maximum size permitted by most systems - the number of connections that can be waiting while the process is handling a particular one
    //if the first argument is a valid socket the call cannot fail
}

void CServer::acceptConnections(){

    int clientSocketDescriptor;
    struct sockaddr_in clientSocketAddress;
    int clientSocketAddressLength = sizeof(clientSocketAddress);

    if((clientSocketDescriptor=accept(serverSocket->getSocketDescriptor(), (struct sockaddr *)&clientSocketAddress, (socklen_t *)&clientSocketAddressLength))==-1){//causes the process to block until a client connects to the server, and then wakes up
        perror("accept");
		_exit(1);
    }
    else{
        printf("Client conectat\n");
        CSocket* client=new CSocket();
        client->setDescriptor(clientSocketDescriptor);
        client->setAddress(clientSocketAddress);
        addClientToList(client);
        readFromClients(); //--thread
    }

}

void CServer::addClientToList(CSocket* clientSocket){
    clientSocketList.push_back(clientSocket);
}

//cred ca thread-ul de read va trebui sa gestioneze si stergerea socket-urilor care nu mai exista

void CServer::readFromClients(){
    char receivedMsg[250];//aici o sa ii schimb size-ul in functie de cum vom face mesajele
    char clientIP[20];
    int clientPort;

    //as putea face o clasa parsator care imi va genera writer-ul necesar pt string ul respectiv, sa fie mai multe thread uri cu cate un writer
    list<CSocket*>::iterator it;
    for(it=clientSocketList.begin(); it!=clientSocketList.end();++it){
        read((*it)->getSocketDescriptor(), receivedMsg, sizeof(receivedMsg)); //the read will block until there is smth to read in the socket, after the client has executed a write() - it returns the number of charachters read
        strcpy(clientIP, inet_ntoa((*it)->getSocketAddress().sin_addr));
        clientPort=ntohs((*it)->getSocketAddress().sin_port);
        printf("Mesaj: %s\nDe la: %s %d\n", receivedMsg, clientIP, clientPort);
        writeToClient((*it)->getSocketDescriptor());
    }
}

void CServer::writeToClient(int socketDescriptor){
    char buffer[30];
    sprintf(buffer, "Message to send back");
    write(socketDescriptor, buffer, sizeof(buffer));
    printf("Press any key to continue...");
    fflush(stdout);
	getchar();
}

void CServer::stop(){
    list<CSocket*>::iterator it;
    for(it=clientSocketList.begin(); it!=clientSocketList.end();++it){
        int descriptor=(*it)->getSocketDescriptor();
        shutdown(descriptor, 2);
        close(descriptor);
    }
    shutdown(serverSocket->getSocketDescriptor(), 2);
    close(serverSocket->getSocketDescriptor());
}