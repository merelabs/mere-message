#include "sender.h"

#include "unistd.h"

Mere::Message::Sender::~Sender()
{

}

Mere::Message::Sender::Sender(QObject *parent) : QObject(parent)
{

}

pid_t Mere::Message::Sender::whoami()
{
    return getpid();
}

