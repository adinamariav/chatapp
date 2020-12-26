#ifndef SINGLECHATWINDOW_H
#define SINGLECHATWINDOW_H

#include "chatconversationhandler.h"
#include "chatlistener.h"

#include <QMainWindow>
#include <QThread>

namespace Ui {
class SIngleChatWindow;
}

class SIngleChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SIngleChatWindow(const QString& usernameFrom, const QString& usernameTo, const int& socket, QWidget *parent = nullptr);
    ~SIngleChatWindow();

public slots:
    void sendAction();
    void receiveAction(QString message);

signals:
    void killHandler();
    void send(QString message);

private:
    Ui::SIngleChatWindow *ui;
    ChatConversationHandler* handler;
    ChatListener* listener;
    QString usernameFrom;
    QString usernameTo;

    void setupConnections();
    void createLogicThread(const int& socket);
    void createListenerThread(const int& socket);
};

#endif // SINGLECHATWINDOW_H
