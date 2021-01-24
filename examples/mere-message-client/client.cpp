#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    m_client = new Mere::Message::Client("/mms");
    connect(m_client, SIGNAL(message(const Mere::Message::Message &)), this, SLOT(message(const Mere::Message::Message &)));
    connect(m_client, SIGNAL(message(const QString &)), this, SLOT(message(const QString &)));
    connect(m_client, SIGNAL(pong(const int &)), this, SLOT(pong(const int &)));

    m_client->join();

    m_client->send("Hi, How are you?");
//    m_client->send("Did you get my last message?");
//    m_client->send("XDid you get my last message?");
//    m_client->send("YDid you get my last message?");
//    m_client->send("ZDid you get my last message?");
//    m_client->send("PDid you get my last message?");
}

void Client::pong(const int &pid)
{
    qDebug() << "PONG";
}

void Client::message(const QString &message)
{
    qDebug() << "MESSAGE BODY ONLY";
}

void Client::message(const Mere::Message::Message &message)
{
    qDebug() << "MESSAGE HEADER AND BODY";
}
