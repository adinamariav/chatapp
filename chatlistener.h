#ifndef CHATLISTENER_H
#define CHATLISTENER_H

#include "communicator.h"
#include <QObject>

class ChatListener : public QObject, public Communicator
{
    Q_OBJECT
public:
    explicit ChatListener(const QString& usernameFrom, const QString& usernameTo, const int& socket, QObject *parent = nullptr);

public slots:
    void process();
    void finish();

signals:
    void finished();
    void receive(QString message);

private:
    QString usernameFrom;
    QString usernameTo;

};

#endif // CHATLISTENER_H
