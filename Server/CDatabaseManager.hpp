#include <mysql/mysql.h>
#include <string>
using namespace std;

class CDatabaseManager{
public:
    CDatabaseManager();
    ~CDatabaseManager();
    CDatabaseManager(string sqlServerUsername, string sqlServerPassword, string sqlServerDatabase);
    string getSignupResponse(string& username, string& password);
    string getLoginResponse(string& username, string& password);
    string getChangeUsernameResponse(string& newusername, string& oldusername);
    string getChangePasswordResponse (string& newpassword, string& username, string& oldpassword);
private:
    MYSQL* con;
    void printError(MYSQL* con);
    bool getCommandExitStatus(string command);
    int getNumberOfSelectedEntries();
    string stringifySignupSelect(string username);
    string stringifySignupInsert(string username, string password);
    string stringifyLoginSelect(string username, string password);
    string stringifyChangeUsernameUpdate(string newusername, string oldusername);
    string stringifyChangePasswordUpdate(string newpassword, string username, string oldpassword);
    void closeConnectionToServer();
};