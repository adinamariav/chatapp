#ifndef CHATWINDOWGENERATOR_H
#define CHATWINDOWGENERATOR_H

#include "singlechatwindow.h"

#include <QThread>
#include <QObject>


class ChatWindowGenerator
{
public:
    ChatWindowGenerator();

    static SIngleChatWindow *getNewChatWindow(const QString& usernameFrom, const QString& usernameTo, const int& socket);
};

#endif // CHATWINDOWGENERATOR_H
