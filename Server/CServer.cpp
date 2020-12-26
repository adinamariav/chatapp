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
#include <signal.h> //pt a gestionea eroarea cauzata de SIGPIPE cand se deconecteaza un socket

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
    databaseManager=new CDatabaseManager("phpmyadmin", "argint99","ChatServer");
    parser=new CParser();
    stopThread1=1;
    stopThread2=1;
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

    databaseManager->closeConnectionToServer();
    delete databaseManager;
    databaseManager=NULL;

    delete parser;
    parser=NULL;
}

CSocket* CServer::getServerSocket(){
    return serverSocket;
}

void CServer::listenForConnections(int maxNumOfConnections){
    listen(serverSocket->getSocketDescriptor(), maxNumOfConnections); 
    //5 is the maximum size permitted by most systems - the number of connections that can be waiting while the process is handling a particular one
    //if the first argument is a valid socket the call cannot fail
}

void* CServer::acceptConnections(){

    int clientSocketDescriptor;
    struct sockaddr_in clientSocketAddress;
    int clientSocketAddressLength = sizeof(clientSocketAddress);

    char clientIP[20];
    int clientPort;

    while(stopThread1){
    if((clientSocketDescriptor=accept(serverSocket->getSocketDescriptor(), (struct sockaddr *)&clientSocketAddress, (socklen_t *)&clientSocketAddressLength))==-1){//causes the process to block until a client connects to the server, and then wakes up
        if(!stopThread1){
            //adica la shutdown nu semnalizeaza nimic
        }
        else{
            perror("Accept error");
		    _exit(1);
        }
    }
    else{
        strcpy(clientIP, inet_ntoa(clientSocketAddress.sin_addr));
        clientPort=ntohs(clientSocketAddress.sin_port);
        printf("Connected %s %d\n", clientIP, clientPort);

        CSocket* client=new CSocket();
        client->setDescriptor(clientSocketDescriptor);
        client->setAddress(clientSocketAddress);
        addClientToList(client);
        addClientToPollSet(clientSocketDescriptor);
    }
    }
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

void* CServer::readFromClients(){
    list<CSocket*>::iterator it;
    while(stopThread2){
        //int setSize=clientPollSet.size(); -- deoarece am modificat codul ca sa stearga descriptorii socketurilor deconectate, trebuie sa las sa recalculeze size-ul, ca sa nu dea eroare in for, cand am size-ul mai mic si incearca sa citeasca de la o locatie mai mare
        //!!!bug interesant
        //!!!cand un socket se deconecteaza, poll crede ca e ceva de citit, citeste ce am trimis ultima oara, si cand se incearca sa se scrie, thread-ul se termina din cauza semnalului SIGPIPE
        //il voi repara prin sigpipe handling, si sterg socketul si din lista de conexiuni si din setul de polling
        //l-am reparat verificand return code-ul de la read
        poll(&clientPollSet[0], clientPollSet.size(), 5); 
        //If timeout is greater than zero, it specifies a maximum interval (in milliseconds) to wait for any file descriptor to become ready. If timeout is zero, then poll() will return without blocking. If the value of timeout is -1, the poll blocks indefinitely.
        //pisi, seteaza si tu timeout-ul pe 5 ms ca sa nu astepti o eternitate ca atunci cand ai pus 60000, *wink, wink #stupid
        //si nu stiai dc mere greu...
        for(int i=0; i<clientPollSet.size();i++){
            if(clientPollSet[i].revents & POLLIN){
                startReading(clientPollSet[i].fd, i);
            }
        }
    }
    pthread_exit(NULL);
}

void CServer::startReading(int fd, int index){
    char receivedMsg[250]; //aici o sa ii schimb size-ul in functie de cum vom face mesajele
    char clientIP[20];
    int clientPort;

    CSocket* clientSocket = findPollFDinList(fd);
    if (!clientSocket) {
        perror("Descriptor not found in pollset");
		_exit(1);
    }

    strcpy(clientIP, inet_ntoa(clientSocket->getSocketAddress().sin_addr));
    clientPort=ntohs(clientSocket->getSocketAddress().sin_port);

    int rc = read(clientSocket->getSocketDescriptor(), receivedMsg, sizeof(receivedMsg)); 
    //the read will block until there is smth to read in the socket, after the client has executed a write() - it returns the number of charachters read            
    
    if (rc==0){
        printf("Disconnected %s %d\n", clientIP, clientPort);
        deleteSocket(clientSocket, index);
    }
    else{
        receivedMsg[rc+1]='\0';
        printf("Message: %s\nFrom: %s %d\n", receivedMsg, clientIP, clientPort);
        processMessage(clientSocket, receivedMsg);
    }
}

void CServer::processMessage(CSocket* clientSocket, char* receivedMsg){  
    string command;

    string operationType;
    parser->getOperationType(receivedMsg, operationType);

    if(operationType=="signup"){
        string username;
        string password;
        parser->splitMessage(username, password);
        writeToClient(clientSocket->getSocketDescriptor(), databaseManager->getSignupResponse(username, password).c_str());
        
    }
    else if(operationType=="login"){
        string username;
        string password;
        parser->splitMessage(username, password);
        writeToClient(clientSocket->getSocketDescriptor(), databaseManager->getLoginResponse(username, password).c_str());
    }    
    else if(operationType=="changeu"){
        string oldusername;
        string newusername;
        string password;
        parser->splitMessage(oldusername, newusername, password);
        writeToClient(clientSocket->getSocketDescriptor(), databaseManager->getChangeUsernameResponse(newusername, oldusername).c_str());
    }    
    else if(operationType=="changep"){
        string username;
        string oldpassword;
        string newpassword;
        parser->splitMessage(username, oldpassword, newpassword);
        writeToClient(clientSocket->getSocketDescriptor(), databaseManager->getChangePasswordResponse(newpassword, username, oldpassword).c_str());
    }   
    else if(operationType=="logout"){
        //string username;
        //parser->getToken(username);
        writeToClient(clientSocket->getSocketDescriptor(), "logout`yes");
    }
    else
        writeToClient(clientSocket->getSocketDescriptor(), "am primit un mesaj ciudat");
    
        //avand in vedere ca aici dau descriptorii ca parametrii si voi lucra cu ei, e posibil ca un client sa se deconecteze inainte sa ii trimit mesajul
        //wut do i do? ma gandeam sa pun actiunea de deconectare if nu stiu ce variabila, sleep(2)
        //ma rog, acum ma gandesc ca daca se deconecteaza, inseamna ca de bunavoie si nesilit de nimeni a acceptat sa nu mai primeasca rasp
        //cu toate astea, daca e offline ar trebui sa ii stochez mesajele undeva si sa i le trimit atunci cand e back online
        //e o poveste pentru alta data...
}

CSocket* CServer::findPollFDinList(int pollDescriptor){
    list<CSocket*>::iterator it;
    for(it=clientSocketList.begin(); it!=clientSocketList.end();++it){
        if((*it)->getSocketDescriptor()==pollDescriptor)
             return *it;
    }
    return NULL;
}

void CServer::deleteSocket(CSocket* clientToDelete, int index){
    clientSocketList.remove(clientToDelete);
    clientPollSet.erase(clientPollSet.begin()+index);
    shutdown(clientToDelete->getSocketDescriptor(), 2);
    close(clientToDelete->getSocketDescriptor());
}

void CServer::writeToClient(int socketDescriptor, string message){
    char buffer[30];
    sprintf(buffer, message.c_str());
    write(socketDescriptor, buffer, sizeof(buffer));
}

void* CServer::readAdminCommands(){
    while(true){
        char command[10];
        scanf("%s", command);
        if(strcmp(command,"exit")==0){ //killareste celelalte thread-uri, mwahahahaha
            stopThread2=0;
            stopThread1=0; 
            //degeaba fac stopThread1=0, fiindca atunci se afla in kernel, si thread-ul e blocat, nu se misca while-ul
            //cum opresc thread-ul ala? aparent nu prea am cum daca e in kernel space
            //sau... trimit un mesaje de la server catre server, se activeaza accept-ul si schimba variabila si kbooooom --nu merge
            //lol, si scria in eroare, accept: Invalid argument Cannot find user-level thread for LWP 6288: generic error  
            stop();
            break;
        }
    }
    pthread_exit(NULL);
}

void CServer::stop(){ //poate avea loc atunci cand clientii sunt conectati, si prin urmare trebuie sa inchidem conexiunile
    list<CSocket*>::iterator it;
    for(it=clientSocketList.begin(); it!=clientSocketList.end();++it){
        int descriptor=(*it)->getSocketDescriptor();
        shutdown(descriptor, 2);
        close(descriptor);
        (*it)=NULL;
    }
    clientSocketList.clear();
    clientPollSet.clear();
    shutdown(serverSocket->getSocketDescriptor(), 2);
    close(serverSocket->getSocketDescriptor()); //va scoate thread-ul cu accept din kernel, dand eroare, iar la verificare stopthread, thread exit, khaching
    //Closing the listening socket is a right way to go. Then accept() returns -1 and sets errno to EBADF, as you already observed. 
    //You just need some more logic in the "threading stuff" to analyze what have actually happened. 
    //For example, test not_ended: if it is false, you know for sure that the error is intended, and that the shutdown is in progress; otherwise bla bla bla
}
