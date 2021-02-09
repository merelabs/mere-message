#include "notifier.h"
#include "messenger.h"

#include "message.h"
#include "command.h"

#include <iostream>

#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>

#include <QSocketNotifier>

#define SOCKET_READER_END 1
#define SOCKET_WRITER_END 0

class Mere::Message::Notifier::NotifierPrivate : public QObject
{
    Q_OBJECT
public:
    ~NotifierPrivate()
    {
        if (m_notifier)
        {
            delete m_notifier;
            m_notifier = nullptr;
        }
    }

    NotifierPrivate(Notifier *q, QObject *parent = nullptr)
        : QObject(parent),
          q_ptr(q)
    {
        if (::socketpair(AF_UNIX, SOCK_STREAM, 0, socketPair))
            qFatal("Couldn't create SIG* socketpair");

        m_notifier = new QSocketNotifier(socketPair[SOCKET_READER_END], QSocketNotifier::Read, this);
        connect(m_notifier, SIGNAL(activated(int)), this, SLOT(notify()));
    }

    int watch(int signal)
    {
        return setup(signal);
    }

    int send(int signal, pid_t pid, int mask)
    {
        union sigval val;
        val.sival_int = mask;

        int result = sigqueue(pid, signal, val);
        if (result == -1)
        {
            std::cerr << "Unable to queue signal; " << strerror(errno) << std::endl;
            switch (errno)
            {
                case EAGAIN:
                    std::cerr << "Process " << getpid() << " does not have priviledge to send signal to " << pid << std::endl;
                    break;

                case EPERM:
                    std::cerr << "Process " << getpid() << " does not have priviledge to send signal to " << pid << std::endl;
                    break;

                case ESRCH:
                    std::cerr << "Please check process with id: " << pid << std::endl;
                    break;
            }
        }
        return result;
    }

private slots:
    void notify()
    {
        m_notifier->setEnabled(false);

        struct {
            int who;
            int what;
        } data;
        ::read(socketPair[SOCKET_READER_END], &data, sizeof(data));

        emit q_ptr->bleep(data.who, data.what);

        m_notifier->setEnabled(true);
    }

private:
    static int setup(int signal)
    {
        struct sigaction signalAction;
        sigemptyset(&signalAction.sa_mask);

        // signalAction.sa_flags = 0;
        signalAction.sa_flags = SA_RESTART | SA_SIGINFO;

        signalAction.sa_sigaction = handler;

        if (sigaction(signal, &signalAction, nullptr))
           return signal;

        return 0;
    }

    static void handler(int signal, siginfo_t *si, void *ucontext)
    {
        Q_UNUSED(signal);
        Q_UNUSED(ucontext);

        // who generate tha signal
        int who  = si->si_pid;

        // mask = metod and related message id (if any)
        int mask = si->si_value.sival_int;
        struct {
            int who;
            int what;
        } data;

        data.who  = who;
        data.what = mask;

        ::write(socketPair[SOCKET_WRITER_END], &data, sizeof(data));
    }

private:
    static int socketPair[2];
    QSocketNotifier *m_notifier;

    Notifier *q_ptr;
};

int Mere::Message::Notifier::NotifierPrivate::socketPair[2] = {0, 0};

Mere::Message::Notifier::~Notifier()
{
    if (d_ptr)
    {
        delete d_ptr;
        d_ptr = nullptr;
    }
}

Mere::Message::Notifier::Notifier(QObject *parent)
    : QObject(parent),
      d_ptr(new NotifierPrivate(this))
{
}

int Mere::Message::Notifier::watch(int signal)
{
    return d_ptr->watch(signal);
}

int Mere::Message::Notifier::send(int signal, pid_t pid, int mask)
{
    return d_ptr->send(signal, pid, mask);
}

#include "notifier.moc"
