#include "server.h"

Server::Server(QObject *parent) : QObject(parent)
{
    m_server = new Mere::Message::Server("/mms");
    connect(m_server, SIGNAL(message(const Mere::Message::Message &)), this, SLOT(message(const Mere::Message::Message &)));
    connect(m_server, SIGNAL(message(const QString &)), this, SLOT(message(const QString &)));
    connect(m_server, SIGNAL(ping(const int &)), this, SLOT(ping(const int &)));

    m_server->start();

//    delete m_server;
}

void Server::ping(const int &pid)
{
    qDebug() << "SOMEONE PING ME" << pid;
}

void Server::message(const QString &message)
{
    qDebug() << "MESSAGE BODY ONLY" << message;
    m_server->send("Hello! I am at your service!");
}

void Server::message(const Mere::Message::Message &message)
{
    qDebug() << "MESSAGE HEADER AND BODY";
    m_server->send("Hello! I am at your service!");
}
