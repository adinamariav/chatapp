#ifndef CHATLISTENER_H
#define CHATLISTENER_H

#include <QObject>
#include <sys/poll.h>
#include "constants.h"

class ChatListener : public QObject
{
    Q_OBJECT
public:
    static ChatListener *getInstance(const int& socket);


public slots:
    void process();
    void finish();

signals:
    void finished();
    void receiveLogin(QString message);
    void receiveLogout(QString message);
    void receiveSignup(QString message);
    void receiveChangeU(QString message);
    void receiveChangeP(QString message);
    void receiveInitU(QString message);
    void receiveInitW(QString message);
    void receiveSearch(QString message);
    void receiveSendM(QString message);
    void receiveMess(QString message);

private:
    //int CSocket;
    pollfd Poll;

    static ChatListener *instance;

    explicit ChatListener(const int& socket, QObject *parent = nullptr);
    QString Listen();
    QStringList SeparateMessage(QString message);
    void emitSignal(const QString& answer);

    void setupConnections();

};

#endif // CHATLISTENER_H
