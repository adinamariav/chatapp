#include "CConnectionManager.hpp"
#include "CDatabaseManager.hpp"
#include "CParser.hpp"
#include <list>
#include <string>
#include <vector>
#include <mysql/mysql.h>
using namespace std;

class CServer {
public:
    static CServer* getInstance(string IPaddress, int portNumber);
    static void destroyInstance();
    void initConnectionSetup();
    void readClientData();
    void* readFromClients(); //returneaza void* si primeste ca arg void*, pt thread, o fct membru implicit primeste ca argument *this pointer
    void readAdminCommands();
    void* manageServer();
private:
    static CServer *instance; 
    CServer(string IPaddress, int portNumber);
    ~CServer();
    CConnectionManager* connectionManager;
    int stopThread2;
    void startReading(int fd, int index);
    CParser* parser;
    CDatabaseManager* databaseManager;
    void processMessage(CSocket* clientSocket, char* receivedMsg);
    void writeToClient(int socketDescriptor, string message);
    void stop();
};