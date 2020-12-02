#ifndef MODIFYNAMEWINDOW_H
#define MODIFYNAMEWINDOW_H

#include <QDialog>

namespace Ui {
class ModifyNameWindow;
}

class ModifyNameWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ModifyNameWindow(QWidget *parent = nullptr);
    ~ModifyNameWindow();

private:
    Ui::ModifyNameWindow *ui;
};

#endif // MODIFYNAMEWINDOW_H
