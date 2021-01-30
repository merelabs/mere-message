#ifndef RECEIVER_H
#define RECEIVER_H

#include "global.h"
#include <QObject>

namespace Mere
{
namespace Message
{

class MERE_MESSAGE_LIB_SPEC Receiver : public QObject
{
    Q_OBJECT
public:
    virtual ~Receiver();
    explicit Receiver(QObject *parent = nullptr);
    virtual void receive(const QString &message) = 0;
};

}
}

#endif // RECEIVER_H
