#include "client.h"
#include "uri.h"

#include <iostream>
#include <unistd.h>
#include <signal.h>

class Mere::Message::Client::ClientPrivate : public QObject
{
    Q_OBJECT
public:
    ~ClientPrivate()
    {
        if(m_messenger)
        {
            delete m_messenger;
            m_messenger = nullptr;
        }
    }

    ClientPrivate(const char *path, Client *q)
        : q_ptr(q)
    {
        Uri uri(path);

        m_messenger = new Messenger(uri.path(), q);

        std::cout << "Its me, a client: " << getpid()  << " connecting to: " << path << std::endl;
    }

    int join()
    {
        m_messenger->notifier()->watch(SIGUSR1);

        int err = m_messenger->join();
        if (!err)
        {
            connect(m_messenger, SIGNAL(pong(const int &)), q_ptr, SIGNAL(pong(const int &)));

            connect(m_messenger, SIGNAL(post(const mid_t &)), q_ptr, SIGNAL(post(const mid_t &)));
            connect(m_messenger, SIGNAL(message(const std::string &)), q_ptr, SIGNAL(message(const std::string &)));
            connect(m_messenger, SIGNAL(message(const Mere::Message::Message &)), q_ptr, SIGNAL(message(const Mere::Message::Message &)));

            connect(m_messenger, SIGNAL(seen(const pid_t &, const mid_t &)), q_ptr, SIGNAL(seen(const pid_t &, const mid_t &)));
            connect(m_messenger, SIGNAL(ackn(const pid_t &, const method_t &)), q_ptr, SIGNAL(ackn(const pid_t &, const method_t &)));
        }

        return err;
    }

    int done()
    {
        return m_messenger->done();
    }

    void ping()
    {
        m_messenger->send(PING);
    }

    void send(const std::string &message)
    {
        m_messenger->send(message.c_str());
    }

    void send(const char *message)
    {
        m_messenger->send(message);
    }

private:
    Messenger *m_messenger;

    Client *q_ptr;

};

Mere::Message::Client::~Client()
{
}

Mere::Message::Client::Client(const std::string &path, QObject *parent)
    : Client(path.c_str(), parent)
{
}

Mere::Message::Client::Client(const char *path, QObject *parent)
    : Sender(parent),
      d_ptr(new ClientPrivate(path, this))
{
}

int Mere::Message::Client::join()
{
    return d_ptr->join();
}

int Mere::Message::Client::done()
{
    return d_ptr->done();
}

void Mere::Message::Client::ping()
{
    return d_ptr->ping();
}

void Mere::Message::Client::send(const std::string &message)
{
    return d_ptr->send(message);
}

#include "client.moc"
