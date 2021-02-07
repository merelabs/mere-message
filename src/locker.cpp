#include "locker.h"

//#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

Locker::Locker(const char *name, QObject *parent)
    : QObject(parent),
      m_name(name)
{

}

int Locker::bind()
{
    m_sem = sem_open(m_name, O_CREAT, 0660, 1);
    if (m_sem == SEM_FAILED)
    {
//        Error::sem_open();
        return errno;
    }

    return 0;
}

int Locker::done()
{
    int err = sem_unlink(m_name);
    if (err == -1)
    {
//        Error::sem_unlink();
    }

    return err;
}

bool Locker::lock()
{
    int err = sem_wait(m_sem);
    if (err == -1)
    {
        //Error::sem_wait();
    }

    return err;
}

bool Locker::free()
{
    int err = sem_post(m_sem);
    if (err == -1)
    {
        //Error::sem_post();
    }

    return err;
}

void Locker::up()
{
//   struct sembuf sb;
//   sb.sem_num = 0;
//   sb.sem_op = 1;
//   sb.sem_flg = 0;

//   semop(m_mutex, &sb, 1);
}
