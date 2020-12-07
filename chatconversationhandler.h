#ifndef CHATCONVERSATIONHANDLER_H
#define CHATCONVERSATIONHANDLER_H

#include <QObject>

class ChatConversationHandler : public QObject
{
    Q_OBJECT
public:
    explicit ChatConversationHandler(QObject *parent = nullptr);

signals:

};

#endif // CHATCONVERSATIONHANDLER_H
