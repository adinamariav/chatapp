#ifndef CHATCONVERSATIONHANDLER_H
#define CHATCONVERSATIONHANDLER_H

#include <QObject>
#include <QMessageBox>
#include "communicator.h"
#include "messagegenerator.h"

class ChatConversationHandler : public QObject, public Communicator
{
    Q_OBJECT
public:
    explicit ChatConversationHandler(const QString& usernameFrom, const QString& usernameTo, int socket, QObject *parent = nullptr);
    const QString &getUsernameTo();

public slots:
    void process();
    void finish();
    void sendMessage(QString message);
    void sendMessageReceived(QString message);
    void receive(QString message);
    void InitMessages();
    void initMessagesReceived(QString message);;

signals:
    void finished();
    void receiveMess(QString message);
    void initMessages(QStringList message);

private:
    QString usernameFrom;
    QString usernameTo;

    void setupConnections();

};

#endif // CHATCONVERSATIONHANDLER_H
