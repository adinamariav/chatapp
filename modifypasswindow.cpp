#include "modifypasswindow.h"
#include "ui_modifypasswindow.h"

ModifyPassWindow::ModifyPassWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModifyPassWindow)
{
    ui->setupUi(this);
}

ModifyPassWindow::~ModifyPassWindow()
{
    delete ui;
}
