#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
    struct sockaddr_in sockserv;
    struct sockaddr_in from;
    int fromlen = sizeof (from);
    char msg[30], buf[30], fromip[20];
    int ss,newsock,i=1,pid=1;

    // verificare argumente
    if(argc<2)
	{
		printf("Eroare - Introduceti IP server ca argument\n");
		printf("Ex.   ./serverinet 127.0.0.1 \n");
		_exit(1);
	}
    
    
    // creare obiect de tip socket pentru server
    if ((ss = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("server: socket");
        _exit(1);
    }

    // atribuire identitate socket server
	sockserv.sin_family=AF_INET;
	sockserv.sin_port=htons(60080);
	inet_aton(argv[1], &sockserv.sin_addr);     //alternativa la functia inet_addr()
    //sockserv.sin_addr.s_addr=inet_addr(argv[1]);
	
	// initializare socket server
    if (bind(ss, (struct sockaddr *)&sockserv, sizeof(sockserv)) < 0)
    {
        perror("server: bind");
        _exit(1);
    }

    // stabilire numar maxim de conexiuni acceptate
	listen(ss,5);
	
	// acceptare conexiuni de la client    
    while(i<=5)
    {
        // acceptare conexiune de la clientul i
        newsock = accept(ss, (struct sockaddr *)&from, (socklen_t *)&fromlen);
        printf("\nClient %d conectat\n",i);
        i++;
        
        // creare proces copil care sa gestioneze conexiunea cu lcientul
        if(pid!=0)
            pid=fork();

        // proces copil -> gestiune conexiune acceptata
        if(pid==0)
        {
            // primire date de la clientul i
            read(newsock,msg,30);

            strcpy(fromip,inet_ntoa(from.sin_addr));
            printf("Mesaj - \" %s \" \n",msg);
            printf("Receptionat de la ip=%s port=%d\n",fromip,ntohs(from.sin_port));
            sleep(1);

            sprintf(buf,"\nOK pid=%d ppid=%d\n",getpid(),getppid());
            write(newsock,buf,sizeof(buf));
            sleep(2);

            shutdown(newsock,2);	// inchidere conexiune cu clientul (inchidere socket client)
	        close(newsock);			// eliberare descriptor socket aferent clientului
        } else {
            // inchidere descriptor socket in procesul parinte (a fost duplicat si preluat de catre procesul copil)
            close(newsock);
        }
    }
    
    sleep(1);
	printf("Press any key to continue...");
	fflush(stdout);
	getchar();
	
	shutdown(ss,2);			// inchidere socket server -> nu mai pot fi acceptate conexiuni
	close(ss);				// eliberare descriptor socket server

    _exit(0);
}