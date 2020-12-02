#include "CSocket.hpp"
#include <list>
#include <string>
using namespace std;

class CServer {
public:
    CServer(string IPaddress, int portNumber);
    ~CServer();
    void listenForConnections(int maxNumOfConnections);
    void acceptConnections();
    void stop();
private:
    CSocket* serverSocket;
    list<CSocket*> clientSocketList;
    CSocket* getServerSocket();
    void addClientToList(CSocket* clientSocket);
    void readFromClients();
    void writeToClient(int socketDescriptor);
};