#include "modifynamewindow.h"
#include "ui_modifynamewindow.h"

ModifyNameWindow::ModifyNameWindow(std::string username, int socket, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModifyNameWindow),
    username(username)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromStdString("Modificare username"));
    setupConnections();
    interface->setSocket(socket);
}

ModifyNameWindow::~ModifyNameWindow()
{
    delete ui;
}

void ModifyNameWindow::okAction()
{
    auto ans = interface->SendMessage(MessageGenerator::modifyUsername(QString::fromStdString(username), ui->nameEdit->text(), ui->passwordEdit->text()));
    auto messagePieces = interface->SeparateMessage(ans);

    if(interface->interpretMessage(messagePieces) == false)
    {
        //throw
        QMessageBox::warning(this, "Fail", "Nu s-a putut schimba username-ul");
    }

    QMessageBox::information(this, "Success", "Username schimbat cu succes!");
    this->hide();
}

void ModifyNameWindow::CancelAction()
{
    this->hide();
}

void ModifyNameWindow::setupConnections()
{
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &ModifyNameWindow::okAction);
    connect(ui->buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &ModifyNameWindow::CancelAction);
}
