#include "messagegenerator.h"



MessageGenerator::MessageGenerator()
{

}

std::string MessageGenerator::logIn(const QString &username, const QString &pass)
{
    std::string mes = Constants::Login + Constants::Separator + username.toStdString() + Constants::Separator + pass.toStdString();

    return mes;
}

std::string MessageGenerator::logOut(const QString &username)
{
    std::string mes = Constants::Logout + Constants::Separator + username.toStdString();

    return mes;
}

std::string MessageGenerator::modifyUsername(const QString &oldUsername, const QString &newUsername, const QString &pass)
{
    std::string mes = Constants::ChangeUsername + Constants::Separator + oldUsername.toStdString() + Constants::Separator + newUsername.toStdString() + Constants::Separator + pass.toStdString();

    return mes;
}

std::string MessageGenerator::modifyPassword(const QString& username, const QString &oldPassword, const QString &newPassword)
{
    std::string mes = Constants::ChangePassword + Constants::Separator +username.toStdString() + Constants::Separator + oldPassword.toStdString() + Constants::Separator + newPassword.toStdString();

    return mes;
}

std::string MessageGenerator::searchUser(const QString &username)
{
    std::string mes = Constants::SearchUsers + Constants::Separator + username.toStdString();

    return mes;
}

std::string MessageGenerator::signUp(const QString &username, const QString &pass)
{
    auto str = Constants::SignUp;
    auto str1 = Constants::Separator;
    auto str2 = username.toStdString();
    auto str3 = pass.toStdString();
    std::string mesaj = str;
    mesaj += str1;
    mesaj+=str2;
    mesaj += str1;
    mesaj += str3;

    return mesaj;
}

std::string MessageGenerator::initUsers(const QString &username)
{
    std::string mes = Constants::InitUsers + Constants::Separator + username.toStdString();

    return mes;

}

std::string MessageGenerator::initWindow(const QString &userRequesting, const QString &userRequested)
{
    std::string mes = Constants::InitWindow + Constants::Separator + userRequesting.toStdString() + Constants::Separator + userRequested.toStdString();

    return mes;
}

std::string MessageGenerator::receiveMessage(const QString &userRequested)
{
    std::string mes = Constants::ReceiveMessage + Constants::Separator + userRequested.toStdString();

    return mes;
}

std::string MessageGenerator::sendMessage(const QString& userRequesting, const QString &userRequested, const QString &message)
{
    std::string mes = Constants::SendMessage + Constants::Separator + userRequesting.toStdString() + Constants::Separator + userRequested.toStdString() + Constants::Separator + message.toStdString();

    return mes;
}
