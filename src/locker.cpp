#include "locker.h"

#include <fcntl.h>
#include <unistd.h>

Mere::Message::Locker::Locker(const char *name)
    : m_name(name)
{

}

int Mere::Message::Locker::bind()
{
    m_sem = sem_open(m_name, O_CREAT, 0660, 1);
    if (m_sem == SEM_FAILED)
    {
//        Error::sem_open();
        return errno;
    }

    return 0;
}

int Mere::Message::Locker::done()
{
    int err = sem_unlink(m_name);
    if (err == -1)
    {
//        Error::sem_unlink();
    }

    return err;
}

bool Mere::Message::Locker::lock()
{
    int err = sem_wait(m_sem);
    if (err == -1)
    {
        //Error::sem_wait();
    }

    return err;
}

bool Mere::Message::Locker::free()
{
    int err = sem_post(m_sem);
    if (err == -1)
    {
        //Error::sem_post();
    }

    return err;
}
