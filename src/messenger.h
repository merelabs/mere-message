#ifndef MERE_MESSAGE_MESSENGER_H
#define MERE_MESSAGE_MESSENGER_H

#include "global.h"
#include "notifier.h"
#include "message.h"
#include "space.h"
#include "id.h"

#include <utility>

#include <QObject>
#include <QDebug>

namespace Mere
{
namespace Message
{

class MERE_MESSAGE_LIB_SPEC Messenger : public QObject, public IDChecker
{
    Q_OBJECT
public:
    virtual ~Messenger();
    explicit Messenger(const char *path, QObject *parent = nullptr);
    explicit Messenger(const std::string &path, QObject *parent = nullptr);

    int bind();
    int join();
    int done();

    Notifier* notifier() const;

    bool ready() const;
    void ready(bool ready);

    /**
     * @brief send
     *
     * Send message to a target or all.
     *
     * Note: Group support is not provided yet!
     *
     * @param data
     * @return 0  - on success
     *         >0 - on failure
     */
    int send(const char *data);
    int send(const char *data, const pid_t target);
    int send(const char *data, const std::vector<pid_t> &targets);

    /**
     * @brief send
     *
     * Send a commnad or request to one or all.
     * Note: Group support is not provided yet!
     *
     * @param data
     * @return 0  - on success
     *         >0 - on failure
     */
    int send(const Method &method);
    int send(const Method &method, const pid_t target);
    int send(const Method &method, const std::vector<pid_t> &targets);

    /**
     * @brief ack
     *
     * Acknowledgement of receiving a message with id.
     *
     * @param id
     * @return 0  - on success
     *         >0 - on failure
     */
    int ackn(pid_t target, Method method);

    bool exists(const mid_t id) const override;

    Message* get(mid_t id) const;

private:
    int write(int signal, int target, const char *data, mid_t *id = nullptr);
    int write(int signal, int target, Method method);

    pid_t origin() const;

    int ping(int signal, int target);
    int pong(int signal, int target);

    bool seen(mid_t id) const;
    bool seen(Message *message) const;

    void notify();
    void notify(int signal);
    void notify(int signal, int target);
    void notify(int signal, int target, Method method);
    void notify(int signal, int target, std::pair<method_t, mid_t> pair) const;

signals:
    // let other party that someone want to make friendship
    void join(const pid_t &pid);

    // let other party know someone what to know if s/he is alive
    void ping(const int &pid);

    // let other party know that i am still alive.
    void pong(const int &pid);

    // let other know a new message is posted
    void post(const mid_t &id);

    // let other know the newly posted message content
    void message(const QString &message);

    // let other know the newly posted message
    void message(const Mere::Message::Message &message);

    // let other party know the receipient seen the message
    void seen(const pid_t &pid, const mid_t &id);

    // let othe party know the receipient know the method call
    void ackn(const pid_t &pid, const method_t &method);

public slots:
    void catched(pid_t who, int what);

private:
    Space m_space;
    Notifier *m_notifier;
};

}
}

#endif // MERE_MESSAGE_MESSENGER_H
