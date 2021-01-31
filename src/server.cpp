#include "server.h"
#include "message.h"
#include "uri.h"

#include <iostream>

Mere::Message::Server::~Server()
{
    if(m_messenger)
    {
        delete m_messenger;
        m_messenger = nullptr;
    }
}

Mere::Message::Server::Server(const std::string &path, QObject *parent)
    : Server(path.c_str(), parent)
{
}

Mere::Message::Server::Server(const char *path, QObject *parent)
    : Sender(parent)
{
    Uri uri(path);

    //std::cout << "Schema:" << uri.schema() << std::endl;
    //std::cout << "Server:" << uri.server() << std::endl;
    //std::cout << "Service:" << uri.service() << std::endl;

    m_messenger = new Messenger(uri.path(), this);
    std::cout << "It's me, a server:" << getpid() << " listening at: " << path << std::endl;
}

int Mere::Message::Server::start()
{
    m_messenger->notifier()->watch(SIGUSR2);

    int err = m_messenger->bind();
    if(!err)
    {
        // handle this......
        //m_messenger->notifier()->watch(SIGCHLD /*SIGCLD*/);

        // listen and accept
        connect(m_messenger, SIGNAL(join(const pid_t &)), this, SLOT(accept(const pid_t&)));;

        connect(m_messenger, SIGNAL(message(const Mere::Message::Message &)), this, SIGNAL(message(const Mere::Message::Message &)));
        connect(m_messenger, SIGNAL(message(const QString &)), this, SIGNAL(message(const QString &)));
        connect(m_messenger, SIGNAL(pong(const int &)), this, SIGNAL(pong(const int &)));

        connect(m_messenger, SIGNAL(seen(const pid_t &, const mid_t &)), this, SIGNAL(seen(const pid_t &, const mid_t &)));
    }

    return err;
}

void Mere::Message::Server::accept(const pid_t &pid)
{
    std::cout << "A process " <<pid << " wanna join in the session." << std::endl;

    auto result = std::find(std::begin(m_clients), std::end(m_clients), pid);
    if(result != std::end(m_clients))
    {
        std::cout << "Oh you already join in the session..." << pid << std::endl;
        return;
    }

    m_clients.push_back(pid);
    std::cout << "A process " << pid << " just joined in the session." << std::endl;

    // acknowledge the acceptance
    m_messenger->ackn(pid, Method::JOIN);
}

int Mere::Message::Server::stop()
{
    return m_messenger->done();
}

void Mere::Message::Server::send(const QString &message)
{
    m_messenger->send(message.toStdString().c_str(), m_clients);
}

void Mere::Message::Server::pong()
{
    m_messenger->send(PONG, m_clients);
}
