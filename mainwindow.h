#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "messagegenerator.h"
#include "communicator.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow, public Communicator
{
    Q_OBJECT

public:
    MainWindow(int socket, QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    int socket;

    void setupConnections(); 

public slots:
    void registerAction();
    void loginAction();
};
#endif // MAINWINDOW_H
