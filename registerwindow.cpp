#include "registerwindow.h"
#include "ui_registerwindow.h"

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
   // interface->initiateConnection();
    auto ans = interface->SendMessage(MessageGenerator::signUp(ui->usernameEdit->text(), ui->passEdit->text()));
    auto messagePieces = interface->SeparateMessage(ans);

    if(interface->interpretMessage(messagePieces) == false)
    {
        //throw
        QMessageBox::warning(this, "Fail", "Nu s-a putut schimba username-ul");
    }

    QMessageBox::information(this, "Success", "Username schimbat cu succes!");
    this->close();
}

void registerWindow::CancelAction()
{

}

void registerWindow::setupConnections()
{
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &registerWindow::okAction);
    connect(ui->buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &registerWindow::CancelAction);
}
