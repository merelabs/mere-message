#ifndef MERE_MESSAGE_NOTIFIER_H
#define MERE_MESSAGE_NOTIFIER_H

#include "global.h"
#include "message.h"
#include "command.h"

#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>

#include <QObject>
#include <QSocketNotifier>
#include <QDebug>

namespace Mere
{
namespace Message
{

class Messenger;

class MERE_MESSAGE_LIB_SPEC Notifier final : public QObject
{
    Q_OBJECT
public:
    ~Notifier();
    Notifier(QObject *parent = nullptr);

    int watch(int signal);

    int send(int signal, pid_t pid, int mask);

private:
    static int setup(int signal);
    static void handler(int signal, siginfo_t *si, void *ucontext);

    static int socketPair[2];

signals:
    void bleep(pid_t pid, int mask);

private slots:
    void notify();

private:
    QSocketNotifier *m_notifier;
};

}
}

#endif // MERE_MESSAGE_NOTIFIER_H
