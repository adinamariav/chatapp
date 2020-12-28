#ifndef MODIFYPASSWINDOW_H
#define MODIFYPASSWINDOW_H

#include <QDialog>
#include <QPushButton>
#include <QMessageBox>

#include "communicator.h"
#include "messagegenerator.h"

namespace Ui {
class ModifyPassWindow;
}

class ModifyPassWindow : public QDialog, public Communicator
{
    Q_OBJECT

public:
    explicit ModifyPassWindow(std::string username, int socket, QWidget *parent = nullptr);
    ~ModifyPassWindow();

public slots:
    void okAction();
    void cancelAction();
    void okActionReceived(QString message);

private:
    Ui::ModifyPassWindow *ui;
    std::string username;

    void setupConnections();
};

#endif // MODIFYPASSWINDOW_H
