#ifndef CHATWINDOWGENERATOR_H
#define CHATWINDOWGENERATOR_H

#include "singlechatwindow.h"

#include <QThread>
#include <QObject>


class ChatWindowGenerator
{
public:
    ChatWindowGenerator();

    static SIngleChatWindow *getNewChatWindow(const QString& username);
};

#endif // CHATWINDOWGENERATOR_H
