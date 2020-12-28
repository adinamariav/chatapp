#include "chatwindow.h"
#include "modifynamewindow.h"
#include "modifypasswindow.h"
#include "mainwindow.h"
#include "ui_chatwindow.h"
#include "chatlistener.h"
#include "chatwindowgenerator.h"

ChatWindow::ChatWindow(std::string username, int socket, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatWindow),
    username(username)
{
    ui->setupUi(this);
    setupConnections();
    interface->setSocket(socket);
    initWindow();
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
    connect(ModNameWindow, SIGNAL(usernameChanged(QString)), this, SLOT(changeUsername(QString)));
    ModNameWindow->show();
    this->show();
}

void ChatWindow::changeUsername(QString newUsername)
{
    this->username = newUsername.toStdString();
}

void ChatWindow::expandAction()
{
    auto listItem = ui->listWidget->currentItem();
    if (listItem != nullptr)
    {
        auto name = listItem->text();
        auto singleWindow = ChatWindowGenerator::getNewChatWindow(QString::fromStdString(this->username), name, interface->getSocket());
        singleWindow->show();
    }

}

void ChatWindow::logoutAction()
{
    interface->SendMessage(MessageGenerator::logOut(QString::fromStdString(username)));
}

void ChatWindow::searchAction()
{
    interface->SendMessage(MessageGenerator::searchUser(ui->lineEdit->text()));
}


void ChatWindow::logoutActionReceived(QString message)
{
    auto messagePieces = interface->SeparateMessage(message.toStdString());

    if(interface->interpretMessage(messagePieces) == false)
    {
        //throw
        QMessageBox::warning(this, "Fail", "Nu s-a putut face delogarea");
    }
    this->close();
    MainWindow* main = new MainWindow(interface->getSocket());

    main->show();
}

void ChatWindow::initActionReceived(QString message)
{
    auto messagePieces = interface->SeparateMessage(message.toStdString());

    if(messagePieces.back() == "firsttime" || messagePieces.back() == "no")
        return;

    auto iter1 = messagePieces.begin();
    iter1++;

    for(auto iter = iter1; iter != messagePieces.end(); iter++)
    {
        auto toDisplay = *iter + ": ";
        iter++;
        toDisplay += *iter;
        auto newItem = new QListWidgetItem(toDisplay, ui->listWidget);
        newItem->setTextAlignment(Qt::AlignmentFlag::AlignRight);
        ui->listWidget->addItem(newItem);
    }
}

void ChatWindow::searchActionReceived(QString message)
{
    ui->listWidget->clear();
    auto messagePieces = interface->SeparateMessage(message.toStdString());

    auto iter1 = messagePieces.begin();
    iter1++;

    for(auto iter = iter1; iter != messagePieces.end(); iter++)
    {
        auto toDisplay = *iter;
        auto newItem = new QListWidgetItem(toDisplay, ui->listWidget);
        //newItem->setTextAlignment(Qt::AlignmentFlag::AlignRight);
        ui->listWidget->addItem(newItem);
    }
}

void ChatWindow::setupConnections()
{
    auto listener = ChatListener::getInstance(interface->getSocket());

    connect(ui->actionModificare_User, &QAction::triggered, this, &ChatWindow::modifyNameAction);
    connect(ui->actionModificare_parola, &QAction::triggered, this, &ChatWindow::modifyPassAction);
    connect(ui->actionLogout, &QAction::triggered, this, &ChatWindow::logoutAction);
    connect(ui->actionExpand, &QAction::triggered, this, &ChatWindow::expandAction);
    connect(ui->searchButton, &QPushButton::clicked, this, &ChatWindow::searchAction);
    connect(listener, SIGNAL(receiveLogout(QString)), this, SLOT(logoutActionReceived(QString)));
    connect(listener, SIGNAL(receiveInitU(QString)), this, SLOT(initActionReceived(QString)));
    connect(listener, SIGNAL(receiveSearch(QString)), this, SLOT(searchActionReceived(QString)));
}

void ChatWindow::initWindow()
{
    interface->SendMessage(MessageGenerator::initUsers(QString::fromStdString(this->username)));
}
