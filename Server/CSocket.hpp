#include <string>
#include <sys/socket.h> //contine definitiile structurilor necesare pentru socket-uri
#include <netinet/in.h> //contine constante si structuri necesare pentru adrese internet
#include <sys/poll.h> //contine structura pollfd care e necesare pentru a intreba in sistem ce fds e gata de citit

using namespace std;

class CSocket{
public:
    CSocket();
    CSocket(string IPaddress, int portNumber);
    int getSocketDescriptor();
    //pollfd& getPollSocketDescriptor();
    sockaddr_in getSocketAddress();
    void setDescriptor(int descriptor);
    void setAddress(sockaddr_in address);
    //void setType(short rwe); //read/write/exception --poll-ul trebuie sa stie pentru ce fel de event sa se uite, ex POLLIN
    //friend bool operator== (CSocket& sock1, CSocket& sock2);
private:
    int socketDescriptor;
    //pollfd socketDescriptor;
    struct sockaddr_in socketAddress; //o structura ce contine adrese internet din netinet/in.h
    void initDescriptor();
    void initAddress(string IPaddress, int portNumber);
    void bindAddressToDescriptor();
};