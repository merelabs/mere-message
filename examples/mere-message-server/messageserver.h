#ifndef MESSAGESERVER_H
#define MESSAGESERVER_H

#include "mere/message/server.h"

#include <QObject>

class MessageServer : public QObject
{
    Q_OBJECT
public:
    explicit MessageServer(QObject *parent = nullptr);

signals:

public slots:
    void ping(const int &pid);
    void message(const QString &message);
    void message(const Mere::Message::Message &message);

private:
    Mere::Message::Server *m_server;
};

#endif // MESSAGESERVER_H
