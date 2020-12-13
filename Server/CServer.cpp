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
#include <sys/poll.h>

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

void* CServer::acceptConnections(){

    int clientSocketDescriptor;
    struct sockaddr_in clientSocketAddress;
    int clientSocketAddressLength = sizeof(clientSocketAddress);

    int index=0;
    while(true){
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
        addClientToPollSet(clientSocketDescriptor);
        index++;
        if(index==3)
            break;

        /*printf("\nLista: ");
        list<CSocket*>::iterator it;
        char clientIP[20];
        for(it=clientSocketList.begin(); it!=clientSocketList.end();++it){
            strcpy(clientIP, inet_ntoa((*it)->getSocketAddress().sin_addr));
            printf("%s ", clientIP);
        }
        printf("\n");
        index++;*/
    }
    }
    printf("S-au conectat 3\n");
    pthread_exit(NULL);
}

void CServer::addClientToList(CSocket* clientSocket){
    clientSocketList.push_back(clientSocket);
}

void CServer::addClientToPollSet(int clientSocketDescriptor){
    pollfd client;
    client.fd=clientSocketDescriptor;
    client.events=POLLIN;
    clientPollSet.push_back(client);
}
//cred ca thread-ul de read va trebui sa gestioneze si stergerea socket-urilor care nu mai exista

/*void* CServer::readFromClients(){
    char receivedMsg[250];//aici o sa ii schimb size-ul in functie de cum vom face mesajele
    char clientIP[20];
    int clientPort;

    //as putea face o clasa parsator care imi va genera writer-ul necesar pt string ul respectiv, sa fie mai multe thread uri cu cate un writer
    list<CSocket*>::iterator it;
    int index=0;
    while(true){
        for(it=clientSocketList.begin(); it!=clientSocketList.end();++it){
            read((*it)->getSocketDescriptor(), receivedMsg, sizeof(receivedMsg)); //the read will block until there is smth to read in the socket, after the client has executed a write() - it returns the number of charachters read
            strcpy(clientIP, inet_ntoa((*it)->getSocketAddress().sin_addr));
            clientPort=ntohs((*it)->getSocketAddress().sin_port);
            printf("Mesaj: %s\nDe la: %s %d\n", receivedMsg, clientIP, clientPort);
            writeToClient((*it)->getSocketDescriptor());
            index++;
            if(index==3)
                break;
        }
        if(index==3)
            break;
    }
    pthread_exit(NULL);
}*/

void* CServer::readFromClients(){
    //as putea face o clasa parsator care imi va genera writer-ul necesar pt string ul respectiv, sa fie mai multe thread uri cu cate un writer
    list<CSocket*>::iterator it;
    int index=0;
    while(true){
        int setSize=clientPollSet.size();
        poll(&clientPollSet[0], setSize, 5); //If timeout is greater than zero, it specifies a maximum interval (in milliseconds) to wait for any file descriptor to become ready. If timeout is zero, then poll() will return without blocking. If the value of timeout is -1, the poll blocks indefinitely.
        //pisi, seteaza si tu timeout-ul pe 5 ms ca sa nu astepti o eternitate ca atunci cand ai pus 60000, *wink, wink #stupid
        //si nu stiai dc mere greu...
        for(int i=0; i<setSize;i++){
            if(clientPollSet[i].revents & POLLIN){
                processRead(clientPollSet[i].fd);
                index++;
                if(index==3)
                    break;
            }
        }
        if(index==3)
            break;
    }
    pthread_exit(NULL);
}

void CServer::processRead(int fd){
    char receivedMsg[250];//aici o sa ii schimb size-ul in functie de cum vom face mesajele
    char clientIP[20];
    int clientPort;

    CSocket* clientSocket = findPollFDinList(fd);
    if (!clientSocket) {
        perror("descriptor not found in pollset");
		_exit(1);
    }

    read(clientSocket->getSocketDescriptor(), receivedMsg, sizeof(receivedMsg)); //the read will block until there is smth to read in the socket, after the client has executed a write() - it returns the number of charachters read            
    strcpy(clientIP, inet_ntoa(clientSocket->getSocketAddress().sin_addr));
    clientPort=ntohs(clientSocket->getSocketAddress().sin_port);
    printf("Mesaj: %s\nDe la: %s %d\n", receivedMsg, clientIP, clientPort);
    writeToClient(clientSocket->getSocketDescriptor());
}

CSocket* CServer::findPollFDinList(int pollDescriptor){
    list<CSocket*>::iterator it;
    for(it=clientSocketList.begin(); it!=clientSocketList.end();++it){
        if((*it)->getSocketDescriptor()==pollDescriptor)
             return *it;
    }
    return NULL;
}

void CServer::writeToClient(int socketDescriptor){
    char buffer[30];
    sprintf(buffer, "Message to send back");
    write(socketDescriptor, buffer, sizeof(buffer));
    //printf("Press any key to continue...");
    //fflush(stdout);
	//getchar();
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
