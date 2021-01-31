#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    m_client = new Mere::Message::Client("mms://local");
    connect(m_client, SIGNAL(message(const Mere::Message::Message &)), this, SLOT(message(const Mere::Message::Message &)));
    connect(m_client, SIGNAL(message(const QString &)), this, SLOT(message(const QString &)));
    connect(m_client, SIGNAL(pong(const int &)), this, SLOT(pong(const int &)));
    connect(m_client, SIGNAL(seen(const pid_t &, const mid_t &)), this, SLOT(seen(const pid_t &, const mid_t &)));

    m_client->join();

    m_client->send("Hi, How are you?");
}

void Client::pong(const int &pid)
{
    qDebug() << "PONG";
}

void Client::message(const QString &message)
{
    qDebug() << "MESSAGE BODY ONLY" << message;
}

void Client::message(const Mere::Message::Message &message)
{
    qDebug() << "MESSAGE HEADER AND BODY";
}

void Client::seen(const pid_t &pid, const mid_t &id)
{
    qDebug() << "MESSAGE WITH ID " << id << " SEEN BY " << pid;

}
