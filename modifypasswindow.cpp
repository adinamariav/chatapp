#include "modifypasswindow.h"
#include "ui_modifypasswindow.h"

ModifyPassWindow::ModifyPassWindow(std::string username, int socket, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModifyPassWindow),
    username(username)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromStdString("Modificare parola"));
    interface->setSocket(socket);
    setupConnections();
}

ModifyPassWindow::~ModifyPassWindow()
{
    delete ui;
}

void ModifyPassWindow::okAction()
{
    auto ans = interface->SendMessage(MessageGenerator::modifyPassword(QString::fromStdString(username), ui->oldPassEdit->text(), ui->newPassEdit->text()));
    auto messagePieces = interface->SeparateMessage(ans);

    if(interface->interpretMessage(messagePieces) == false)
    {
        //throw
        QMessageBox::warning(this, "Fail", "Nu s-a putut schimba parola");
    }

    QMessageBox::information(this, "Success", "Parola schimbata cu succes!");
    this->hide();
}

void ModifyPassWindow::cancelAction()
{
    this->hide();
}

void ModifyPassWindow::setupConnections()
{
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &ModifyPassWindow::okAction);
    connect(ui->buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &ModifyPassWindow::cancelAction);
}
