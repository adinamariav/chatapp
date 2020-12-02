#include "modifynamewindow.h"
#include "ui_modifynamewindow.h"

ModifyNameWindow::ModifyNameWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModifyNameWindow)
{
    ui->setupUi(this);
}

ModifyNameWindow::~ModifyNameWindow()
{
    delete ui;
}
