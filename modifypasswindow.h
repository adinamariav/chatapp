#ifndef MODIFYPASSWINDOW_H
#define MODIFYPASSWINDOW_H

#include <QDialog>

namespace Ui {
class ModifyPassWindow;
}

class ModifyPassWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ModifyPassWindow(QWidget *parent = nullptr);
    ~ModifyPassWindow();

private:
    Ui::ModifyPassWindow *ui;
};

#endif // MODIFYPASSWINDOW_H
