#ifndef LOCKER_H
#define LOCKER_H

#include "global.h"
#include <semaphore.h>

#include <QObject>

namespace Mere
{
namespace Message
{

class MERE_MESSAGE_LIB_SPEC Locker
{
public:
    explicit Locker(const char *name);

    int bind();
    int done();

    bool lock();
    bool free();

private:
    const char *m_name;
    sem_t *m_sem;
};

}
}

#endif // LOCKER_H
