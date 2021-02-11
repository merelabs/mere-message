#include "client.h"
#include <iostream>

Client::~Client()
{
    if(m_client)
    {
        delete m_client;
        m_client = nullptr;
    }
}

Client::Client(QObject *parent) : QObject(parent)
{
    m_client = new Mere::Message::Client("mms://lets-talk");
    connect(m_client, SIGNAL(message(const Mere::Message::Message &)), this, SLOT(message(const Mere::Message::Message &)));
    connect(m_client, SIGNAL(message(const std::string &)), this, SLOT(message(const std::string &)));
    connect(m_client, SIGNAL(pong(const int &)), this, SLOT(pong(const int &)));
    connect(m_client, SIGNAL(seen(const pid_t &, const mid_t &)), this, SLOT(seen(const pid_t &, const mid_t &)));

    int err = m_client->join();
    if (err)
    {
        std::cout << "Sorry, failed to connect with server runnit at : " << "mms://lets-talk" << std::endl;
        return;
    }

    std::cout << "Connected with server runnit at : " << "mms://lets-talk" << std::endl;
    m_client->send("Hi, How are you?");
}

void Client::pong(const int &pid)
{
    std::cout << "Got response of my PING" << std::endl;
}

void Client::message(const std::string &message)
{
    std::cout << "Someone sends me message: " << message << std::endl;
}

void Client::message(const Mere::Message::Message &message)
{
    std::cout << "Someone sends me message: " << message._dblock.data << std::endl;
}

void Client::seen(const pid_t &pid, const mid_t &id)
{
    std::cout << "Someone seen my message: " << pid << id << std::endl;
}
