#include "CSocket.hpp"
#include <list>
#include <string>
using namespace std;

class CServer {
public:
    static CServer* getInstance(string IPaddress, int portNumber);
    static void destroyInstance();
    void listenForConnections(int maxNumOfConnections);
    void acceptConnections();
    void stop();
private:
    static CServer *instance; 
    CServer(string IPaddress, int portNumber);
    ~CServer();
    CSocket* serverSocket;
    list<CSocket*> clientSocketList;
    CSocket* getServerSocket();
    void addClientToList(CSocket* clientSocket);
    void readFromClients();
    void writeToClient(int socketDescriptor);
};