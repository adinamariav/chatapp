#include "singlechatwindow.h"
#include "ui_singlechatwindow.h"

SIngleChatWindow::SIngleChatWindow(const QString& usernameFrom, const QString& usernameTo, const int& socket, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SIngleChatWindow),
    usernameFrom(usernameFrom),
    usernameTo(usernameTo)
{
    ui->setupUi(this);
    setupConnections();
    createLogicThread(socket);
    createListenerThread(socket);
}

SIngleChatWindow::~SIngleChatWindow()
{
    emit killHandler();
    delete ui;
    delete handler;
}

void SIngleChatWindow::sendAction()
{    
    auto message = ui->messageEdit->toPlainText();
    ui->messageEdit->clear();
    emit send(message);
    auto newItem = new QListWidgetItem(message, ui->listWidget);
    newItem->setTextAlignment(Qt::AlignmentFlag::AlignRight);
    ui->listWidget->addItem(newItem);
    //ACTUAL SENDING is done in the logical thread
}

void SIngleChatWindow::receiveAction(QString message)
{
    QString mess = usernameFrom + ": " + message;
    auto newItem = new QListWidgetItem(mess, ui->listWidget);
    ui->listWidget->addItem(newItem);
}

void SIngleChatWindow::setupConnections()
{
    connect(ui->sendButton, &QPushButton::clicked, this, &SIngleChatWindow::sendAction);
    connect(this, SIGNAL(send(QString)), handler, SLOT(ChatConversationHandler::sendMessage(QString)));
    connect(handler, SIGNAL(receive(QString)), this, SLOT(receiveAction()));
}

void SIngleChatWindow::createLogicThread(const int& socket)
{
    QThread* thread = new QThread;
    this->handler = new ChatConversationHandler(this->usernameFrom, usernameTo, socket);
    handler->moveToThread(thread);

    QObject::connect(thread, SIGNAL(started()), handler, SLOT(process()));
    QObject::connect(this, SIGNAL(killHandler()), handler, SLOT(finish()));
    QObject::connect(handler, SIGNAL(finished()), thread, SLOT(quit()));
    QObject::connect(handler, SIGNAL(finished()), handler, SLOT(deleteLater()));
    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}

void SIngleChatWindow::createListenerThread(const int& socket)
{
    QThread* thread = new QThread;
    this->listener = new ChatListener(usernameFrom, usernameTo, socket);
    handler->moveToThread(thread);

    QObject::connect(thread, SIGNAL(started()), listener, SLOT(process()));
    QObject::connect(this, SIGNAL(killHandler()), listener, SLOT(finish()));
    QObject::connect(listener, SIGNAL(finished()), thread, SLOT(quit()));
    QObject::connect(listener, SIGNAL(finished()), listener, SLOT(deleteLater()));
    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}

