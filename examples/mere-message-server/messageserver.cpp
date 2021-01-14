#include "messageserver.h"

MessageServer::MessageServer(QObject *parent) : QObject(parent)
{
    m_server = new Mere::Message::Server("/mms");
    connect(m_server, SIGNAL(message(const Mere::Message::Message &)), this, SLOT(message(const Mere::Message::Message &)));
    connect(m_server, SIGNAL(message(const QString &)), this, SLOT(message(const QString &)));
    connect(m_server, SIGNAL(ping(const int &)), this, SLOT(ping(const int &)));

    m_server->start();

//    delete m_server;
}

void MessageServer::ping(const int &pid)
{
    qDebug() << "SOMEONE PING ME" << pid;
}

void MessageServer::message(const QString &message)
{
    qDebug() << "MESSAGE BODY ONLY" << message;
}

void MessageServer::message(const Mere::Message::Message &message)
{
    qDebug() << "MESSAGE HEADER AND BODY";
}
