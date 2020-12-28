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

typedef void * (*THREADFUNCPTR)(void *); //cast-ul tipului de pointer pt functia thread-ului
#define MAX 5

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
    connectionManager=new CConnectionManager(IPaddress, portNumber);
    databaseManager=new CDatabaseManager("phpmyadmin", "argint99","ChatServer");
    parser=new CParser();
    stopThread2=1;
}

CServer::~CServer(){
    delete connectionManager;
    connectionManager=NULL;

    delete databaseManager;
    databaseManager=NULL;

    delete parser;
    parser=NULL;
}

void CServer::initConnectionSetup(){
    connectionManager->listenForConnections(MAX);  
    pthread_t thread1; 
    //se blocheaza in accept pana cand apare o conexiune noua pe care o adauga in lista de conexiuni si in poll set
    //si din cauza ca e in kernel majoritatea timpului, cand celelalte thread-uri dau exit, asta da accept invalid argument
    int rc1=pthread_create(&thread1, NULL, (THREADFUNCPTR) &CConnectionManager::acceptConnections, (void*) connectionManager);
    if (rc1) {
        printf("ERROR; return code from pthread_create() is %d\n", rc1);
        _exit(1);
    }
}

void CServer::readClientData(){
    pthread_t thread2; 
    //while infinit care verifica lista de conexiuni si poll setul si citeste; chiar daca s-au deconectat toti utilizatorii, merge in continuare
    int rc2=pthread_create(&thread2, NULL, (THREADFUNCPTR) &CServer::readFromClients, (void*) this);
    if (rc2) {
        printf("ERROR; return code from pthread_create() is %d\n", rc2);
        _exit(1);
    }
    //C++ class member functions have a hidden this parameter passed in
    //pthread_create() has no idea what value of this to use
    //you have to use a static class method (which has no this parameter), or a plain ordinary function to bootstrap the class
}

void CServer::readAdminCommands(){
    pthread_t thread3;
    //permite administrarea server-ului, in principal inchiderea
    int rc3=pthread_create(&thread3, NULL, (THREADFUNCPTR) &CServer::manageServer, (void*) this);
    if (rc3) {
        printf("ERROR; return code from pthread_create() is %d\n", rc3);
        _exit(1);
    }
    pthread_join(thread3, NULL);
    //wait till threads are complete before main continues
}

void* CServer::readFromClients(){
    list<CSocket*>::iterator it;
    while(stopThread2){
        //int setSize=clientPollSet.size(); -- deoarece am modificat codul ca sa stearga descriptorii socketurilor deconectate, trebuie sa las sa recalculeze size-ul, ca sa nu dea eroare in for, cand am size-ul mai mic si incearca sa citeasca de la o locatie mai mare
        //!!!bug interesant
        //!!!cand un socket se deconecteaza, poll crede ca e ceva de citit, citeste ce am trimis ultima oara, si cand se incearca sa se scrie, thread-ul se termina din cauza semnalului SIGPIPE
        //il voi repara prin sigpipe handling, si sterg socketul si din lista de conexiuni si din setul de polling
        //l-am reparat verificand return code-ul de la read
        vector<pollfd>& clientPollSet=connectionManager->getClientPollSet();
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

    CSocket* clientSocket = connectionManager->findPollFDinList(fd);
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
        connectionManager->deleteSocket(clientSocket, index);
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
        pair<string, int> p;
        p.first=username;
        p.second=clientSocket->getSocketDescriptor();
        connectionManager->insertPairToUserSocketMap(p);
        writeToClient(clientSocket->getSocketDescriptor(), databaseManager->getSignupResponse(username, password).c_str());
    }
    else if(operationType=="login"){
        string username;
        string password;
        parser->splitMessage(username, password);
        pair<string, int> p;
        p.first=username;
        p.second=clientSocket->getSocketDescriptor();
        connectionManager->insertPairToUserSocketMap(p);
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
        string username;
        parser->splitMessage(username);
        connectionManager->deleteUserPairFromMap(username);
        writeToClient(clientSocket->getSocketDescriptor(), "logout`yes");
    }
    else if(operationType=="initmain"){
        string username;
        parser->splitMessage(username);
        writeToClient(clientSocket->getSocketDescriptor(), databaseManager->getInitMainWindowResponse(username).c_str());
    }
    else if(operationType=="initmess"){
        string userRequesting;
        string userRequested;
        parser->splitMessage(userRequesting, userRequested);
        writeToClient(clientSocket->getSocketDescriptor(), databaseManager->getInitMessageWindowResponse(userRequesting, userRequested).c_str());
    }
    else if(operationType=="search"){
        string username;
        parser->splitMessage(username);
        writeToClient(clientSocket->getSocketDescriptor(), databaseManager->getSearchRequestResponse(username).c_str());
    }
    else if(operationType=="send"){
        string userRequesting;
        string userRequested;
        string message;
        parser->splitMessage(userRequesting, userRequested, message);
        writeToClient(clientSocket->getSocketDescriptor(), databaseManager->getSendMessageRequestResponse(userRequesting, userRequested, message));
        writeToClient(connectionManager->getClientSocketDescriptorBasedOnUsername(userRequested), databaseManager->getReceiveMessageServiceResponse(userRequesting, userRequested, message));
    }
    else
        writeToClient(clientSocket->getSocketDescriptor(), "am primit un mesaj ciudat");
    
        //avand in vedere ca aici dau descriptorii ca parametrii si voi lucra cu ei, e posibil ca un client sa se deconecteze inainte sa ii trimit mesajul
        //wut do i do? ma gandeam sa pun actiunea de deconectare if nu stiu ce variabila, sleep(2)
        //ma rog, acum ma gandesc ca daca se deconecteaza, inseamna ca de bunavoie si nesilit de nimeni a acceptat sa nu mai primeasca rasp
        //cu toate astea, daca e offline ar trebui sa ii stochez mesajele undeva si sa i le trimit atunci cand e back online
        //e o poveste pentru alta data...
}

void CServer::writeToClient(int socketDescriptor, string message){
    char buffer[30];
    sprintf(buffer, message.c_str());
    write(socketDescriptor, buffer, sizeof(buffer));
}

void* CServer::manageServer(){
    while(true){
        char command[10];
        scanf("%s", command);
        if(strcmp(command,"exit")==0){ //killareste celelalte thread-uri, mwahahahaha
            stopThread2=0;
            connectionManager->setStopThread1(0); 
            //degeaba fac stopThread1=0, fiindca atunci se afla in kernel, si thread-ul e blocat, nu se misca while-ul
            //cum opresc thread-ul ala? aparent nu prea am cum daca e in kernel space
            //sau... trimit un mesaje de la server catre server, se activeaza accept-ul si schimba variabila si kbooooom --nu merge
            //lol, si scria in eroare, accept: Invalid argument Cannot find user-level thread for LWP 6288: generic error  
            break;
        }
    }
    pthread_exit(NULL);
}
