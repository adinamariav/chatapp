#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include "communicator.h"


namespace Ui {
class ChatWindow;
}

class ChatWindow : public QMainWindow, public Communicator
{
    Q_OBJECT

public:
    explicit ChatWindow(std::string username, int socket, QWidget *parent = nullptr);
    ~ChatWindow();

public slots:
    void modifyPassAction();
    void modifyNameAction();
    void logoutAction();

private:
    Ui::ChatWindow *ui;
    std::string username;

    void setupConnections();
    void initWindow();
};

#endif // CHATWINDOW_H
