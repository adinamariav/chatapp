#include "registerwindow.h"
#include "ui_registerwindow.h"
#include "chatlistener.h"

registerWindow::registerWindow(int socket, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::registerWindow)
{
    ui->setupUi(this);
    interface->setSocket(socket);
    setupConnections();
}

registerWindow::~registerWindow()
{
    delete ui;
}

void registerWindow::okAction()
{
    interface->SendMessage(MessageGenerator::signUp(ui->usernameEdit->text(), ui->passEdit->text()));
}

void registerWindow::CancelAction()
{
    this->close();
}

void registerWindow::okActionReceived(QString message)
{
    auto messagePieces = interface->SeparateMessage(message.toStdString());

    if(interface->interpretMessage(messagePieces) == false)
    {
        //throw
        QMessageBox::warning(this, "Fail", "Nu s-a putut schimba username-ul");
    }

    QMessageBox::information(this, "Success", "Username schimbat cu succes!");
    this->close();
}

void registerWindow::setupConnections()
{
    auto listener = ChatListener::getInstance(interface->getSocket());
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &registerWindow::okAction);
    connect(ui->buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &registerWindow::CancelAction);
    connect(listener, SIGNAL(receiveSignup(QString)), this, SLOT(okActionReceived(QString)));

}
