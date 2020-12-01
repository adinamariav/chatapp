#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
	struct sockaddr_in sockcl;
	struct sockaddr_in to;
	char buf[]="testing testing 123...";
	int cs;

	// verificare argumente
	if(argc<3)
	{
		printf("Eroare - Introduceti IP client si IP server ca argumente\n");
		printf("Ex.   ./client 192.168.0.3 127.0.0.1 \n");
		_exit(1);
	}
	
	// creare obiect de tip socket pentru client
	if ((cs = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("client: socket");
		_exit(1);
	}
	
	// atribuire identitate socket client
	sockcl.sin_family=AF_INET;
	sockcl.sin_addr.s_addr=inet_addr(argv[1]);
	
	// atribuire identitate socket server
	to.sin_family=AF_INET;
	to.sin_port = htons(64000);
	to.sin_addr.s_addr = inet_addr(argv[2]);
	
	// initializare socket client
	if (bind(cs, (struct sockaddr *)&sockcl, sizeof(sockcl)) < 0)
	{
		perror("client: bind");
		_exit(1);
	}
	// din acest moment, socketul client este activ
	
	// initiere conexiune cu serverul
	if (connect(cs,(struct sockaddr *)&to, sizeof(to)) < 0)
	{
		perror("client: connect");
		_exit(1);
	}

	printf("Conectare cu succes\n");
	
	// transmitere date catre server
	write(cs,buf,sizeof(buf));
	sleep(2);
	printf("Mesaj trimis cu succes\n");

	// primire confirmare de la server, DACA este cazul
	read(cs,buf,sizeof(buf));
	printf("Confirmare server - \" %s \" \n",buf);

	printf("Press any key to continue...");
	fflush(stdout);
	getchar();

	shutdown(cs,2);		// inchidere conexiune/socket client
	close(cs);			// eliberare descriptor socket
	_exit(0);
}