#include "client.h"

#include <iostream>

Mere::Message::Client::~Client()
{
    if(m_messenger)
    {
        delete m_messenger;
        m_messenger = nullptr;
    }
}

Mere::Message::Client::Client(const char *name, QObject *parent)
    : Sender(parent),
      m_messenger(new Messenger(name, this))
{
    std::cout << "Its me, a client:" << getpid() << std::endl;
}

int Mere::Message::Client::join()
{
    m_messenger->notifier()->watch(SIGUSR1);

    int err = m_messenger->join();
    if (!err)
    {
        connect(m_messenger, SIGNAL(pong(const int &)), this, SIGNAL(pong(const int &)));

        connect(m_messenger, SIGNAL(post(const mid_t &)), this, SIGNAL(post(const mid_t &)));
        connect(m_messenger, SIGNAL(message(const QString &)), this, SIGNAL(message(const QString &)));
        connect(m_messenger, SIGNAL(message(const Mere::Message::Message &)), this, SIGNAL(message(const Mere::Message::Message &)));

        connect(m_messenger, SIGNAL(seen(const pid_t &, const mid_t &)), this, SIGNAL(seen(const pid_t &, const mid_t &)));
        connect(m_messenger, SIGNAL(ackn(const pid_t &, const metod_t &)), this, SIGNAL(ackn(const pid_t &, const metod_t &)));
    }

    return err;
}

int Mere::Message::Client::done()
{
    return m_messenger->done();
}

void Mere::Message::Client::ping()
{
    m_messenger->send(PING);
}

void Mere::Message::Client::send(const QString &message)
{
    m_messenger->send(message.toStdString().c_str());
}
