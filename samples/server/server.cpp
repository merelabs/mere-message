#include "server.h"
#include <iostream>

Server::~Server()
{
    if (m_server)
    {
        delete m_server;
        m_server = nullptr;
    }
}

Server::Server(QObject *parent) : QObject(parent)
{
    m_server = new Mere::Message::Server("mms://lets-talk");
    connect(m_server, SIGNAL(message(const Mere::Message::Message &)), this, SLOT(message(const Mere::Message::Message &)));
    connect(m_server, SIGNAL(message(const std::string &)), this, SLOT(message(const std::string &)));
    connect(m_server, SIGNAL(ping(const int &)), this, SLOT(ping(const int &)));

    m_server->start();
}

void Server::ping(const int &pid)
{
    std::cout << "Someone PING me!" << pid;
}

void Server::message(const std::string &message)
{
    std::cout << "Someone sends message: " << message << std::endl;
    m_server->send("Hello! I am at your service!");
}

void Server::message(const Mere::Message::Message &message)
{
    std::cout << "Someone sends message: " << message._dblock.data << std::endl;
    m_server->send("Hello! I am at your service!");
}
