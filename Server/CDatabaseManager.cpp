#include "CDatabaseManager.hpp"

CDatabaseManager::CDatabaseManager(){

}

CDatabaseManager::~CDatabaseManager(){
    closeConnectionToServer();
}

CDatabaseManager::CDatabaseManager(string sqlServerUsername, string sqlServerPassword, string sqlServerDatabase){
    con = mysql_init(NULL);

    if (con == NULL){
      fprintf(stderr, "%s\n", mysql_error(con));
      exit(1);
    }
    
    if (mysql_real_connect(con, "localhost", sqlServerUsername.c_str(), sqlServerPassword.c_str(), sqlServerDatabase.c_str(), 0, NULL, 0) == NULL){
      printError(con);
    }

    if (mysql_query(con, "SELECT * FROM Users")){
      printError(con);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL){
      printError(con);
    }

    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;

    while ((row = mysql_fetch_row(result))){
      for(int i = 0; i < num_fields; i++){
          printf("%s ", row[i] ? row[i] : "NULL");
      }
      printf("\n");
    }

    mysql_free_result(result);
}

void CDatabaseManager::printError(MYSQL* con){
    fprintf(stderr, "%s\n", mysql_error(con));
    //mysql_close(con);
    //exit(1);
}

void CDatabaseManager::closeConnectionToServer(){
    mysql_close(con);
}

bool CDatabaseManager::getCommandExitStatus(string command){
    if (mysql_query(con, command.c_str())){
        printError(con);
        return false;
    }
    return true;
}

int CDatabaseManager::getNumberOfSelectedEntries(){
    MYSQL_RES *result = mysql_store_result(con);
    return mysql_num_rows(result);
}

string CDatabaseManager::stringifySignupSelect(string username){
    string command="SELECT Username FROM Users WHERE Username='";
    command+=username;
    command+="'";
    printf("%s\n", command.c_str());
    return command;
}

string CDatabaseManager::stringifyLoginSelect(string username, string password){
    string command="SELECT Username, Password FROM Users WHERE Username='";
    command+=username;
    command+="' AND Password='";
    command+=password;
    command+="'";
    printf("%s\n", command.c_str());
    return command;
}

string CDatabaseManager::stringifySignupInsert(string username, string password){
    string command="INSERT INTO Users VALUES('";
    command+=username;
    command+="','";
    command+=password;
    command+="',0)";
    printf("%s\n", command.c_str());
    return command;
}

string CDatabaseManager::stringifyChangeUsernameUpdate(string newusername, string oldusername){
    string command="UPDATE Users SET Username='";
    command+=newusername;
    command+="' WHERE Username='";
    command+=oldusername;
    command+="'";
    printf("%s\n", command.c_str());
    return command;
}

string CDatabaseManager::stringifyChangePasswordUpdate(string newpassword, string username, string oldpassword){
    string command="UPDATE Users SET Password='";
    command+=newpassword;
    command+="' WHERE Username='";
    command+=username;
    command+="' AND Password='";
    command+=oldpassword;
    command+="'";
    printf("%s\n", command.c_str());
    return command;
}

string CDatabaseManager::stringifyInitMainWindowSelect(string username){
    /*string command="SELECT FromUsername, Message, MAX(Date) AS LastDate FROM Messages WHERE ToUsername='";
    command+=username;
    command+="' GROUP BY FromUsername ORDER BY LastDate DESC";*/
    /*string command="SELECT M.FromUsername, M.Message FROM Messages AS M WHERE M.Date IN (SELECT TOP 1 Mes.Date FROM Messages AS Mes WHERE Mes.FromUsername=M.FromUsername) AND M.ToUsername='";
    command+=username;
    command+="'";*/
    string command="SELECT M.FromUsername, M.Message, M.Date FROM Messages AS M INNER JOIN (SELECT FromUsername, MAX(Date) AS MaxDate FROM Messages GROUP BY FromUsername)Mes ON M.FromUsername=Mes.FromUsername AND M.Date=Mes.MaxDate WHERE M.ToUsername='";
    command+=username;
    command+="'";
    printf("%s\n", command.c_str());
    return command;
}

string CDatabaseManager::stringifyInitMessageWindowSelect(string userRequesting, string userRequested){
    string command="SELECT FromUsername, Message FROM Messages WHERE FromUsername='";
    command+=userRequested;
    command+="' AND ToUsername='";
    command+=userRequesting;
    command+="' OR FromUsername='";
    command+=userRequesting;
    command+="' AND ToUsername='";
    command+=userRequested;
    command+="' ORDER BY Date";
    printf("%s\n", command.c_str()); 
    return command;
}

string CDatabaseManager::stringifySearchRequestSelect(string username){
    string command="SELECT Username FROM Users WHERE Username LIKE '%";
    command+=username;
    command+="%'";
    printf("%s\n", command.c_str());
    return command;
}

string CDatabaseManager::stringifySendMessageRequestInsert(string userRequesting, string userRequested, string message){
    string command="INSERT INTO Messages VALUES(NULL,'";
    command+=userRequesting;
    command+="','";
    command+=userRequested;
    command+="','";
    command+=message;
    command+="',curdate(),0)";
    printf("%s\n", command.c_str());
    return command;
}

string CDatabaseManager::getSignupResponse(string& username, string& password){
    //verifica daca mai exista username-ul
    //getSignUpResponse, returneaza un string
    string command=stringifySignupSelect(username);
    if(!getCommandExitStatus(command)){
        return "signup`no";
    }
    else{
        if(getNumberOfSelectedEntries())
            return "signup`no"; 
        else {
            command=stringifySignupInsert(username, password);
            if(!getCommandExitStatus(command))
                return "signup`no";
            else{
                return "signup`yes";
            }           
        }
    } 
}

string CDatabaseManager::getLoginResponse(string& username, string& password){
    //verifica daca mai exista username-ul
    string command=stringifyLoginSelect(username, password);
    if(!getCommandExitStatus(command)){
        return "login`no";
    }
    else{
        if(getNumberOfSelectedEntries())
            return "login`yes";
        else {
            return "login`no";
        }    
    } 
}

string CDatabaseManager::getChangeUsernameResponse(string& newusername, string& oldusername){
    string command=stringifyChangeUsernameUpdate(newusername, oldusername);
    if(!getCommandExitStatus(command)){
        return "changeu`no";
    }
    else{
        if(mysql_affected_rows(con))
            return "changeu`yes";
        else
            return "changeu`no";
    }
}

string CDatabaseManager::getChangePasswordResponse (string& newpassword, string& username, string& oldpassword){
    string command=stringifyChangePasswordUpdate(newpassword, username, oldpassword);
    if(!getCommandExitStatus(command)){
        return "changep`no";
    }
    else{
        if(mysql_affected_rows(con))
            return "changep`yes";
        else
            return "changep`no";
    }
}

string CDatabaseManager::getInitMainWindowResponse(string& username){
    string command=stringifyInitMainWindowSelect(username);
    if(!getCommandExitStatus(command)){
        return "initmain`no";
    }
    else{
        //if(!getNumberOfSelectedEntries()) --dadea nul rezultatul daca faceam asta
            //return "initmain`firsttime"; 
        MYSQL_RES *result = mysql_store_result(con);
        int numRows=mysql_num_rows(result);
        if(!numRows)
            return "initmain`firsttime"; 
        else {
            if (result == NULL){
                printError(con);
            }
            int num_fields = mysql_num_fields(result);
            num_fields--; //nu vreau sa imi puna in raspuns si data maxima, nu ma intere
            MYSQL_ROW row;
            string response="initmain`";
            int index=0;
            while ((row = mysql_fetch_row(result))){
                for(int i = 0; i < num_fields; i++){
                    index++;
                    printf("%s ", row[i] ? row[i] : "NULL");
                    response+=row[i];
                    if(index<numRows || index==1)
                        response+="`"; 
                }
                printf("\n");
            }
            mysql_free_result(result);
            return response;          
        }
    } 
}

string CDatabaseManager::getInitMessageWindowResponse(string& userRequesting, string& userRequested){
    string command=stringifyInitMessageWindowSelect(userRequesting, userRequested);
    if(!getCommandExitStatus(command)){
        return "initmess`no";
    }
    else{
        MYSQL_RES *result = mysql_store_result(con);
        int numRows=mysql_num_rows(result);
        if(!numRows)
            return "initmess`firsttime"; 
        else {
            if (result == NULL){
                printError(con);
            }
            int num_fields = mysql_num_fields(result);
            MYSQL_ROW row;
            string response;
            int index=0;
            while ((row = mysql_fetch_row(result))){
                index++;
                for(int i = 0; i < num_fields; i++){
                    printf("%s ", row[i] ? row[i] : "NULL");
                    response+=row[i];
                    if(index<numRows)
                        response+="`";
                }
                printf("\n");
            }
            mysql_free_result(result);
            return response;           
        }
    } 
}

string CDatabaseManager::getSearchRequestResponse(string& username){
    string command=stringifySearchRequestSelect(username);
    if(!getCommandExitStatus(command)){
        return "search`no";
    }
    else{
        MYSQL_RES *result = mysql_store_result(con);
        int numRows=mysql_num_rows(result);
        if(!numRows)
            return "search`no"; 
        else {
            if (result == NULL){
                printError(con);
            }
            int num_fields = mysql_num_fields(result);
            MYSQL_ROW row;
            string response="search`";
            int index=0;
            while ((row = mysql_fetch_row(result))){
                index++;
                for(int i = 0; i < num_fields; i++){
                    printf("%s ", row[i] ? row[i] : "NULL");
                    response+=row[i];
                    if(index<numRows)
                        response+="`";
                }
                printf("\n");
            }
            mysql_free_result(result);
            return response;           
        }
    } 
}

string CDatabaseManager::getSendMessageRequestResponse(string& userRequesting, string& userRequested, string& message){
    string command=stringifySendMessageRequestInsert(userRequesting, userRequested, message);
    if(!getCommandExitStatus(command)){
        return "message`no";
    }
    else
        return "message`yes";
}

//nu uita de offline messages, sa pun la login un update al statusului, la true, la logout la false, cand se deconecteaza brusc, false
string CDatabaseManager::getReceiveMessageServiceResponse(string& userRequesting, string& userRequested, string& message){
    string response = "receive`";
    response+=userRequesting;
    response+="`";
    response+=message;
    return response;
}