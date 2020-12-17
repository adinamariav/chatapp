#ifndef CHATLISTENER_H
#define CHATLISTENER_H

#include <QObject>

class ChatListener : public QObject
{
    Q_OBJECT
public:
    explicit ChatListener(QObject *parent = nullptr);

signals:

};

#endif // CHATLISTENER_H
