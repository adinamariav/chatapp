#include <stdio.h> 
#include <stdlib.h> //pt exit
#include <string.h>
#include <unistd.h> //read si write
#include <sys/types.h> //contine definitiile diferitor tipuri de date folosite in apelurile de sistem
#include <sys/socket.h> //contine definitiile structurilor necesare pentru socket-uri
#include <netinet/in.h> //contine constante si structuri necesare pentru adrese internet
#include <arpa/inet.h> //inet_addr, inet_ntoa

int main(int argc, char** argv){

    if(argc<2){
        printf("Eroare - Introduceti IP server ca argument\n");
        printf("Ex. ./myserver 127.0.0.1\n");
        _exit(1);
    }

    int serverSocketDescriptor;
    int clientSocketDescriptor;

    struct sockaddr_in serverSocketAddress; //o structura ce contine adrese internet din netinet/in.h
    serverSocketAddress.sin_family=AF_INET;
    serverSocketAddress.sin_port=htons(64000); //it is necessary to convert this integer from host byte order to network byte order
    serverSocketAddress.sin_addr.s_addr=inet_addr(argv[1]); //function from arpa/inet.h - the IP address of the machine on which the server is running, INADDR_ANY - constant that gets this address
    
    struct sockaddr_in clientSocketAddress;
    int clientSocketAddressLength = sizeof(clientSocketAddress);

    serverSocketDescriptor = socket(AF_INET, SOCK_STREAM, 0); //socket system call
    //AF_INET the address domain of the socket
    //SOCK_STREAM charachters are read in a continuous stream, as if from a file
    //PROTOCOL-if 0, the OS will choose TCP sockets for stream sockets and UDP for datagram sockets
    if(serverSocketDescriptor<0){
        perror("Socket server nu a putut fi creat"); //it displays a message about the error on stderr
        _exit(1);
    }

    if(bind(serverSocketDescriptor, (struct sockaddr *)&serverSocketAddress, sizeof(serverSocketAddress))<0){
        perror("Nu s-a realizat bind-ul");
        _exit(1);
    }

    listen(serverSocketDescriptor, 5); //5 is the maximum size permitted by most systems - the number of connections that can be waiting while the process is handling a particular one
    //if the first argument is a valid socket the call cannot fail

    if((clientSocketDescriptor=accept(serverSocketDescriptor, (struct sockaddr *)&clientSocketAddress, (socklen_t *)&clientSocketAddressLength))==-1){//causes the process to block until a client connects to the server, and then wakes up
        perror("accept");
		_exit(1);
    }

    printf("\nClient conectat\n");
    sleep(2);

    char receivedMsg[30], buffer[30], clientIP[20];
    int clientPort;

    read(clientSocketDescriptor, receivedMsg, sizeof(receivedMsg)); //the read will block until there is smth to read in the socket, after the client has executed a write() - it returns the number of charachters read
    strcpy(clientIP, inet_ntoa(clientSocketAddress.sin_addr));
    clientPort=ntohs(clientSocketAddress.sin_port);
    printf("Mesaj: %s\nDe la: %s %d\n", receivedMsg, clientIP, clientPort);
    sleep(1);

    sprintf(buffer, "Message to send back");
    write(clientSocketDescriptor, buffer, sizeof(buffer));

    sleep(1);
    printf("Press any key to continue...");
    fflush(stdout);
	getchar();

    shutdown(clientSocketDescriptor, 2);
    close(clientSocketDescriptor);

    shutdown(serverSocketDescriptor, 2);
    close(serverSocketDescriptor);

    _exit(0);

}