#ifndef SINGLECHATWINDOW_H
#define SINGLECHATWINDOW_H

#include "chatconversationhandler.h"
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
    void initMessagesReceived(QStringList message);

signals:
    void killHandler();
    void send(QString message);
    void InitMessages();

private:
    Ui::SIngleChatWindow *ui;
    ChatConversationHandler* handler;
    QString usernameFrom;
    QString usernameTo;

    void setupConnections();
    void createLogicThread(const int& socket);
    void initMessages();
};

#endif // SINGLECHATWINDOW_H
