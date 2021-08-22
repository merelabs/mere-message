#ifndef MESSAGECLIENT_H
#define MESSAGECLIENT_H

#include "mere/message/client.h"

class Client : public QObject
{
    Q_OBJECT
public:
    ~Client();
    explicit Client(QObject *parent = nullptr);

signals:

public slots:
    void pong(const int &pid);
    void message(const std::string &message);
    void message(const Mere::Message::Message &message);

    void seen(const pid_t &pid, const mid_t &id);

private:
    Mere::Message::Client *m_client;
};

#endif // MESSAGECLIENT_H
