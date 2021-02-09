#ifndef MERE_MESSAGE_NOTIFIER_H
#define MERE_MESSAGE_NOTIFIER_H

#include "global.h"

#include <QObject>

namespace Mere
{
namespace Message
{

class MERE_MESSAGE_LIB_SPEC Notifier final : public QObject
{
    Q_OBJECT
public:
    ~Notifier();
    Notifier(QObject *parent = nullptr);
    int watch(int signal);
    int send(int signal, pid_t pid, int mask);

signals:
    void bleep(pid_t pid, int mask);

private:
    class NotifierPrivate;
    NotifierPrivate *d_ptr;
};

}
}

#endif // MERE_MESSAGE_NOTIFIER_H
