#include "CSocket.hpp"
#include <list>
#include <string>
#include <vector>
using namespace std;

class CServer {
public:
    static CServer* getInstance(string IPaddress, int portNumber);
    static void destroyInstance();
    void listenForConnections(int maxNumOfConnections);
    void* acceptConnections(); //daca nu ar fi fost un thread, atunci nu am fi ajuns la read in main fiindca e un while infinit
    void* readFromClients(); //returneaza void* si primeste ca arg void*, pt thread, o fct membru implicit primeste ca argument *this pointer
    void stop();
private:
    static CServer *instance; 
    CServer(string IPaddress, int portNumber);
    ~CServer();
    CSocket* serverSocket;
    list<CSocket*> clientSocketList;
    vector<pollfd> clientPollSet;
    CSocket* getServerSocket();
    void addClientToList(CSocket* clientSocket);
    void addClientToPollSet(int clientSocketDescriptor);
    void writeToClient(int socketDescriptor);
    CSocket* findPollFDinList(int pollDescriptor);
    void processRead(int fd);
};