#include "CServer.hpp"
#include <unistd.h>
#include <pthread.h>

struct thread_data {
    int thread_id;
    CServer* server; //inainte aveam referinta si spunea the default constructor cannot be refernced it is a deleted function
    //it's not the struct that doesn't have a default constructor, it's an object in it without one
};

void* acceptConnections(void* thread1_data){
    struct thread_data local_data;
    local_data=*(struct thread_data*)thread1_data;
    long tid;
    tid=(long)local_data.thread_id;
    printf("\nHello! It's me, thread #%ld\n", tid);

    local_data.server->acceptConnections();

    pthread_exit(NULL);
}

int main(int argc, char** argv){

    if(argc<2){
        printf("Eroare - Introduceti IP server ca argument\n");
        printf("Ex. ./server 127.0.0.1\n");
        _exit(1);
    }

    CServer* server=CServer::getInstance(argv[1], 64000);

    server->listenForConnections(5);

    pthread_t thread1;
    struct thread_data thread1_data;
    thread1_data.thread_id=1;
    thread1_data.server=server;
    /*int rc=pthread_create(&thread1, NULL, acceptConnections, (void*)&thread1_data);
    if (rc) {
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        _exit(1);
    }*/
    //C++ class member functions have a hidden this parameter passed in
    //pthread_create() has no idea what value of this to use
    //you have to use a static class method (which has no this parameter), or a plain ordinary function to bootstrap the class
    server->acceptConnections();//--thread

    server->stop();

    server->destroyInstance();

    //pthread_exit(NULL);
    _exit(0);
}