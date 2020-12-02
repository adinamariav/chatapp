#include "singlechatwindow.h"
#include "ui_singlechatwindow.h"

SIngleChatWindow::SIngleChatWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SIngleChatWindow)
{
    ui->setupUi(this);
}

SIngleChatWindow::~SIngleChatWindow()
{
    delete ui;
}
