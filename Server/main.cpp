#include "CServer.hpp"
#include <unistd.h>
#include <pthread.h>
#include <mysql/mysql.h>

typedef void * (*THREADFUNCPTR)(void *); //cast-ul tipului de pointer pt functia thread-ului

int main(int argc, char** argv){

    if(argc<2){
        printf("Error - Enter server IP as an argument\n");
        printf("Ex. ./server 127.0.0.1\n");
        _exit(1);
    }

    CServer* server=CServer::getInstance(argv[1], 64000);

    server->getReadyForConnectingToClients();

    pthread_t thread2; 
    //while infinit care verifica lista de conexiuni si poll setul si citeste; chiar daca s-au deconectat toti utilizatorii, merge in continuare
    int rc2=pthread_create(&thread2, NULL, (THREADFUNCPTR) &CServer::readFromClients, (void*) server);
    if (rc2) {
        printf("ERROR; return code from pthread_create() is %d\n", rc2);
        _exit(1);
    }
    //C++ class member functions have a hidden this parameter passed in
    //pthread_create() has no idea what value of this to use
    //you have to use a static class method (which has no this parameter), or a plain ordinary function to bootstrap the class

    pthread_t thread3;
    //permite administrarea server-ului, in principal inchiderea
    int rc3=pthread_create(&thread3, NULL, (THREADFUNCPTR) &CServer::readAdminCommands, (void*) server);
    if (rc3) {
        printf("ERROR; return code from pthread_create() is %d\n", rc3);
        _exit(1);
    }

    pthread_join(thread3, NULL);
    //wait till threads are complete before main continues
    
    server->destroyInstance();

    pthread_exit(NULL);
}