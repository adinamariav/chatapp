#ifndef SINGLECHATWINDOW_H
#define SINGLECHATWINDOW_H

#include <QMainWindow>

namespace Ui {
class SIngleChatWindow;
}

class SIngleChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SIngleChatWindow(QWidget *parent = nullptr);
    ~SIngleChatWindow();

private:
    Ui::SIngleChatWindow *ui;
};

#endif // SINGLECHATWINDOW_H
