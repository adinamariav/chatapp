#ifndef MESSAGEGENERATOR_H
#define MESSAGEGENERATOR_H
#include <QString>
#include "constants.h"

class MessageGenerator
{
public:
    MessageGenerator();
    static std::string logIn(const QString& username, const QString& pass);
    static std::string logOut(const QString& username);
    static std::string modifyUsername(const QString& oldUsername, const QString& newUsername, const QString& pass);
    static std::string modifyPassword(const QString& username, const QString& oldPassword, const QString& newPassword);
    static std::string searchUser(const QString& username);
    static std::string signUp(const QString& username, const QString& pass);
    static std::string initUsers(const QString& username);
    static std::string initWindow(const QString& userRequesting, const QString& userRequested);
    static std::string receiveMessage(const QString& userRequested);
    static std::string sendMessage(const QString& userRequesting, const QString& userRequested, const QString& message);
};

#endif // MESSAGEGENERATOR_H
