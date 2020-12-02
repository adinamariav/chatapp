#include "CServer.hpp"
#include <unistd.h>

int main(int argc, char** argv){

    if(argc<2){
        printf("Eroare - Introduceti IP server ca argument\n");
        printf("Ex. ./myserver 127.0.0.1\n");
        _exit(1);
    }

    CServer server(argv[1], 64000);

    server.listenForConnections(5);
    server.acceptConnections();
    server.stop();

    _exit(0);
}