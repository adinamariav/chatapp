#include "CServer.hpp"
#include <unistd.h>
#include <pthread.h>

typedef void * (*THREADFUNCPTR)(void *); //cast-ul tipului de pointer pt functia thread-ului

int main(int argc, char** argv){

    if(argc<2){
        printf("Eroare - Introduceti IP server ca argument\n");
        printf("Ex. ./server 127.0.0.1\n");
        _exit(1);
    }

    CServer* server=CServer::getInstance(argv[1], 64000);

    server->listenForConnections(5);

    pthread_t thread1;
    int rc1=pthread_create(&thread1, NULL, (THREADFUNCPTR) &CServer::acceptConnections, (void*) server);
    if (rc1) {
        printf("ERROR; return code from pthread_create() is %d\n", rc1);
        _exit(1);
    }

    pthread_t thread2;
    int rc2=pthread_create(&thread2, NULL, (THREADFUNCPTR) &CServer::readFromClients, (void*) server);
    if (rc2) {
        printf("ERROR; return code from pthread_create() is %d\n", rc2);
        _exit(1);
    }
    //C++ class member functions have a hidden this parameter passed in
    //pthread_create() has no idea what value of this to use
    //you have to use a static class method (which has no this parameter), or a plain ordinary function to bootstrap the class

    //wait till threads are complete before main continues
    pthread_join(thread2, NULL);
    
    server->stop();

    server->destroyInstance();

    pthread_exit(NULL);
    //_exit(0);
}