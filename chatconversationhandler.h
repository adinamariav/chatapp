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

signals:
    void finished();

private:
    QString usernameFrom;
    QString usernameTo;

};

#endif // CHATCONVERSATIONHANDLER_H
