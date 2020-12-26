#include "CDatabaseManager.hpp"

CDatabaseManager::CDatabaseManager(){

}

CDatabaseManager::~CDatabaseManager(){

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
    return command;
}

string CDatabaseManager::stringifyLoginSelect(string username, string password){
    string command="SELECT Username, Password FROM Users WHERE Username='";
    command+=username;
    command+="' AND Password='";
    command+=password;
    command+="'";
    return command;
}

string CDatabaseManager::stringifySignupInsert(string username, string password){
    string command="INSERT INTO Users VALUES(NULL,'";
    printf("%s\n", command.c_str());
    command+=username;
    command+="','";
    command+=password;
    command+="')";
    return command;
}

string CDatabaseManager::stringifyChangeUsernameUpdate(string newusername, string oldusername){
    string command="UPDATE Users SET Username='";
    command+=newusername;
    command+="' WHERE Username='";
    command+=oldusername;
    command+="'";
    return command;
}

string CDatabaseManager::stringifyChangePasswordUpdate(string newpassword, string username, string oldpassword){
    string command="UPDATE Users SET Password='";
    command+=newpassword;
    command+="' WHERE Username='";
    command+=username;
    command+="' and Password='";
    command+=oldpassword;
    command+="'";
    return command;
}

string CDatabaseManager::getSignupResponse(string& username, string& password){
    //verifica daca mai exista username-ul
    //getSignUpResponse, returneaza un string
    string command=stringifySignupSelect(username);
    printf("%s\n", command.c_str());
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
        return "changeu`yes";
    }
}

string CDatabaseManager::getChangePasswordResponse (string& newpassword, string& username, string& oldpassword){
    string command=stringifyChangePasswordUpdate(newpassword, username, oldpassword);
    if(!getCommandExitStatus(command)){
        return "changep`no";
    }
    else{
        return "changep`yes";
    }
}