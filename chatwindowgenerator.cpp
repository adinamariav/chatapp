#include "chatwindowgenerator.h"

ChatWindowGenerator::ChatWindowGenerator()
{

}

SIngleChatWindow *ChatWindowGenerator::getNewChatWindow(const QString &usernameFrom, const QString &usernameTo, const int &socket)
{
    SIngleChatWindow* window = new SIngleChatWindow(usernameFrom, usernameTo, socket);

    return window;
}
