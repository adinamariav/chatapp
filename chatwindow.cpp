#include "chatwindow.h"
#include "modifynamewindow.h"
#include "modifypasswindow.h"
#include "mainwindow.h"
#include "ui_chatwindow.h"

ChatWindow::ChatWindow(std::string username, int socket, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatWindow),
    username(username)
{
    ui->setupUi(this);
    setupConnections();
    interface->setSocket(socket);
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::modifyPassAction()
{    
    ModifyPassWindow *ModPassWindow = new ModifyPassWindow(username, interface->getSocket(), this);

    ModPassWindow->show();
    this->show();
}

void ChatWindow::modifyNameAction()
{
    ModifyNameWindow *ModNameWindow = new ModifyNameWindow(username, interface->getSocket(), this);

    ModNameWindow->show();
    this->show();
}

void ChatWindow::logoutAction()
{
    auto ans = interface->SendMessage(MessageGenerator::logOut(QString::fromStdString(username)));
    auto messagePieces = interface->SeparateMessage(ans);

    if(interface->interpretMessage(messagePieces) == false)
    {
        //throw
        QMessageBox::warning(this, "Fail", "Nu s-a putut face delogarea");
    }
    this->close();
    MainWindow* main = new MainWindow(interface->getSocket());

    main->show();
}

void ChatWindow::setupConnections()
{
    connect(ui->actionModificare_User, &QAction::triggered, this, &ChatWindow::modifyNameAction);
    connect(ui->actionModificare_parola, &QAction::triggered, this, &ChatWindow::modifyPassAction);
    connect(ui->actionLogout, &QAction::triggered, this, &ChatWindow::logoutAction);
}

void ChatWindow::initWindow()
{
    auto ans = interface->SendMessage(MessageGenerator::initUsers(QString::fromStdString(this->username)));
    auto messagePieces = interface->SeparateMessage(ans);

    for(auto mess : messagePieces)
    {

    }
}
