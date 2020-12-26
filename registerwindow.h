#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QDialog>
#include <QPushButton>
#include <QMessageBox>

#include "communicator.h"
#include "messagegenerator.h"

namespace Ui {
class registerWindow;
}

class registerWindow : public QDialog, public Communicator
{
    Q_OBJECT

public:
    explicit registerWindow(int socket, QWidget *parent = nullptr);
    ~registerWindow();

public slots:
    void okAction();
    void CancelAction();

private:
    Ui::registerWindow *ui;

    void setupConnections();
};

#endif // REGISTERWINDOW_H
