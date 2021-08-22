#include "server.h"
#include "message.h"
#include "uri.h"

#include <cstring>
#include <iostream>
#include <unistd.h>
#include <signal.h>

class Mere::Message::Server::ServerPrivate : public QObject
{
    Q_OBJECT
public:
    ~ServerPrivate()
    {
        if(m_messenger)
        {
            delete m_messenger;
            m_messenger = nullptr;
        }
    }

    ServerPrivate(const char *path, Server *q, QObject *parent = nullptr)
        : q_ptr(q)
    {
        Uri uri(path);
        m_messenger = new Messenger(uri.path(), q);
        std::cout << "It's me, a server: " << getpid() << " listening at: " << path << std::endl;
    }


    int start()
    {
        m_messenger->notifier()->watch(SIGUSR2);

        int err = m_messenger->bind();
        if(!err)
        {
            // handle this......
            //m_messenger->notifier()->watch(SIGCHLD /*SIGCLD*/);

            // listen and accept
            connect(m_messenger, SIGNAL(join(const pid_t &)), this, SLOT(accept(const pid_t&)));;

            connect(m_messenger, SIGNAL(message(const Mere::Message::Message &)), q_ptr, SIGNAL(message(const Mere::Message::Message &)));
            connect(m_messenger, SIGNAL(message(const std::string &)), q_ptr, SIGNAL(message(const std::string &)));
            connect(m_messenger, SIGNAL(pong(const int &)), q_ptr, SIGNAL(pong(const int &)));

            connect(m_messenger, SIGNAL(seen(const pid_t &, const mid_t &)), q_ptr, SIGNAL(seen(const pid_t &, const mid_t &)));
        }

        return err;
    }

    int stop()
    {
        return m_messenger->done();
    }

    void send(const std::string &message)
    {
        m_messenger->send(message.c_str(), m_clients);
    }

    void pong()
    {
        m_messenger->send(PONG, m_clients);
    }

private slots:
    void accept(const pid_t &pid)
    {
        std::cout << "A process " << pid << " wanna join in the session." << std::endl;

        auto result = std::find(std::begin(m_clients), std::end(m_clients), pid);
        if(result != std::end(m_clients))
        {
            std::cout << "Oh you already join in the session..." << pid << std::endl;
            return;
        }

        m_clients.push_back(pid);
        std::cout << "A process " << pid << " just joined in the session." << std::endl;

        m_messenger->get(1);

        // acknowledge the acceptance
        m_messenger->ackn(pid, Method::JOIN);
    }

private:
    std::vector<int> m_clients;

    Messenger *m_messenger;

    Server *q_ptr;
};

Mere::Message::Server::~Server()
{
    if (d_ptr)
    {
        delete d_ptr;
        d_ptr = nullptr;
    }
}

Mere::Message::Server::Server(const std::string &path, QObject *parent)
    : Server(path.c_str(), parent)
{
}

Mere::Message::Server::Server(const char *path, QObject *parent)
    : Sender(parent),
      d_ptr(new ServerPrivate(path, this))
{
}

int Mere::Message::Server::start()
{
    return d_ptr->start();
}

int Mere::Message::Server::stop()
{
    return d_ptr->stop();
}

void Mere::Message::Server::send(const std::string &message)
{
    return d_ptr->send(message);
}

void Mere::Message::Server::pong()
{
    return d_ptr->pong();
}

#include "server.moc"
