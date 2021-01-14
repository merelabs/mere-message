#include "messageclient.h"

MessageClient::MessageClient(QObject *parent) : QObject(parent)
{
    m_client = new Mere::Message::Client("/mms");
    connect(m_client, SIGNAL(message(const Mere::Message::Message &)), this, SLOT(message(const Mere::Message::Message &)));
    connect(m_client, SIGNAL(message(const QString &)), this, SLOT(message(const QString &)));
    connect(m_client, SIGNAL(pong(const int &)), this, SLOT(pong(const int &)));

    m_client->join();
}

void MessageClient::pong(const int &pid)
{
    qDebug() << "PONG";
}

void MessageClient::message(const QString &message)
{
    qDebug() << "MESSAGE BODY ONLY";
}

void MessageClient::message(const Mere::Message::Message &message)
{
    qDebug() << "MESSAGE HEADER AND BODY";
}
