#ifndef MESSAGECLIENT_H
#define MESSAGECLIENT_H

#include "mere/message/client.h"

#include <QObject>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);

signals:

public slots:
    void pong(const int &pid);
    void message(const QString &message);
    void message(const Mere::Message::Message &message);

private:
    Mere::Message::Client *m_client;
};

#endif // MESSAGECLIENT_H
