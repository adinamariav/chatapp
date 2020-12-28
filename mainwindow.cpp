#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "registerwindow.h"
#include "chatwindow.h"
#include "chatlistener.h"

MainWindow::MainWindow(int socket, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    interface->setSocket(socket);
    setupConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupConnections()
{
    auto listener = ChatListener::getInstance(interface->getSocket());
    connect(ui->registerButton, &QPushButton::clicked, this, &MainWindow::registerAction);
    connect(ui->logInButton, &QPushButton::clicked, this, &MainWindow::loginAction);
    connect(listener, SIGNAL(receiveLogin(QString)), this, SLOT(loginActionReceived(QString)));
}

void MainWindow::registerAction()
{
    registerWindow *regWindow = new registerWindow(interface->getSocket(), this);
    regWindow->show();
}

void MainWindow::loginAction()
{
    interface->SendMessage(MessageGenerator::logIn(ui->userLineEdit->text(), ui->passLineEdit->text()));
}

void MainWindow::loginActionReceived(QString message)
{
    auto processedAns = interface->SeparateMessage(message.toStdString());

    if(interface->interpretMessage(processedAns) == false)
    {
        //throw exception
        QMessageBox::warning(this, "Fail", "Nu s-a putut realiza logarea");
    }

    QMessageBox::information(this, "Success", "Logare realizata cu succes!");
    ChatWindow *chWindow = new ChatWindow(ui->userLineEdit->text().toStdString(), interface->getSocket());
    this->hide();
    chWindow->show();
}



