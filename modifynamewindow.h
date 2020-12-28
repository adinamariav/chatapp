#ifndef MODIFYNAMEWINDOW_H
#define MODIFYNAMEWINDOW_H

#include <QDialog>
#include <QPushButton>
#include <QMessageBox>
#include "communicator.h"
#include "messagegenerator.h"

namespace Ui {
class ModifyNameWindow;
}

class ModifyNameWindow : public QDialog, public Communicator
{
    Q_OBJECT

public:
    explicit ModifyNameWindow(std::string username, int socket, QWidget *parent = nullptr);
    ~ModifyNameWindow();

public slots:
    void okAction();
    void CancelAction();
    void okActionReceived(QString message);

signals:
    void usernameChanged(QString newUsername);

private:
    Ui::ModifyNameWindow *ui;
    std::string username;

    void setupConnections();
};

#endif // MODIFYNAMEWINDOW_H
