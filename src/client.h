#ifndef MERE_MESSAGE_CLIENT_H
#define MERE_MESSAGE_CLIENT_H

#include "global.h"
#include "messenger.h"
#include "sender.h"

namespace Mere
{
namespace Message
{

class MERE_MESSAGE_LIB_SPEC Client : public Sender
{
    Q_OBJECT
public:
    virtual ~Client();
    explicit Client(const char *path, QObject *parent = nullptr);

    int join();
    int done();

    void ping();
    void send(const QString &message) override;

signals:
    void call(const int &pid);

    void ping(const int &pid);
    void pong(const int &pid);

    void post(const int &id);

    void message(const QString &message);
    void message(const Mere::Message::Message &message);

    void seen(const pid_t &pid, const mid_t &id);
    void ackn(const pid_t &pid, const method_t &method);

private:
    bool m_ready = false;
    Messenger *m_messenger;
};

}
}

#endif // MERE_MESSAGE_CLIENT_H
