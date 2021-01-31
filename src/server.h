#ifndef MERE_MESSAGE_SERVER_H
#define MERE_MESSAGE_SERVER_H

#include "global.h"
#include "messenger.h"
#include "sender.h"

#include <QDebug>

namespace Mere
{
namespace Message
{


class MERE_MESSAGE_LIB_SPEC Server : public Sender
{
    Q_OBJECT
public:
    ~Server();
    explicit Server(const std::string &path, QObject *parent = nullptr);
    explicit Server(const char *path, QObject *parent = nullptr);
    int start();
    int stop();

    void pong();
    void send(const QString &message) override;

private slots:
    void accept(const pid_t &pid);

signals:
    void join(const int &pid);
    void ping(const int &pid);
    void pong(const int &pid);

    void message(const QString &message);
    void message(const Mere::Message::Message &message);

    void post(const mid_t &id);

    void seen(const pid_t &pid, const mid_t &id);

private:
    std::vector<int> m_clients;

    Messenger *m_messenger;
};

}
}

#endif // MERE_MESSAGE_SERVER_H
