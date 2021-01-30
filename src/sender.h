#ifndef MERE_MESSAGE_SENDER_H
#define MERE_MESSAGE_SENDER_H

#include "global.h"
#include <QObject>

namespace Mere
{
namespace Message
{

class MERE_MESSAGE_LIB_SPEC Sender : public QObject
{
    Q_OBJECT
public:
    virtual ~Sender();
    explicit Sender(QObject *parent = nullptr);

    virtual pid_t whoami();

    virtual void send(const QString &message) = 0;

};

}
}

#endif // MERE_MESSAGE_SENDER_H
