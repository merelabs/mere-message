#include "space.h"
#include "error.h"

#include <iostream>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

class Mere::Message::Space::SpacePrivate
{
public:
    ~SpacePrivate()
    {
        if(m_name)
        {
            delete m_name;
            m_name = nullptr;
        }
        done();

        if (m_locker)
        {
            m_locker->done();
            delete m_locker;
            m_locker = nullptr;
        }
    }

    SpacePrivate(const char *name, int unit, int size, Space *q)
        : m_unit(unit),
          m_size(size),
          q_ptr(q)
    {
        m_name = (char *) malloc(strlen(name) + 1);
        strcpy(m_name, name);

        m_locker = new Locker(m_name);
    }


    int bind()
    {
        m_shm = shm_open(m_name, O_CREAT /*| O_EXCL*/ | O_RDWR , 0600);
        if (m_shm == -1)
        {
            Error::shm_open();
            return errno;
        }

        int err = ftruncate(m_shm, size());
        if (err)
        {
            shm_unlink(m_name);
            Error::ftruncate();
            return errno;
        }

        vmap();

        m_space->origin = getpid();
        m_space->head   = 0;

        ready(true);

        // lock
        err = m_locker->bind();
        if (err) done();

        return err;
    }

    int join()
    {
        m_shm = shm_open(m_name, O_RDWR , 0600);
        if (m_shm == -1)
        {
            Error::shm_open();
            return errno;
        }

        vmap();
        ready(true);

        // lock
        int err = m_locker->bind();
        if (err) done();

        return err;
    }


    int done()
    {
        if(!ready()) return 1;

        munmap(m_space, size());
        close(m_shm);
        shm_unlink(m_name);

        ready(false);

        return 0;
    }

    pid_t origin() const
    {
        return m_space->origin;
    }

    Mere::Message::Message* prev() const
    {
        m_space->head = (m_space->head - 1 + m_unit) % m_unit;

        return get();
    }

    Mere::Message::Message* head() const
    {
        return get();
    }

    Mere::Message::Message* next() const
    {
        m_space->head = (m_space->head + 1) % m_unit;

        return get();
    }

    Mere::Message::Message* get() const
    {
        return get(m_space->head);
    }

    Mere::Message::Message* get(unsigned int index) const
    {
        if (index >= m_unit)
            throw std::invalid_argument("No more space for this unit of message.");

    //    return m_space->messages + index;
        return &m_space->messages[index];
    }

    int set(const Message &message) const
    {
        return this->set(m_space->head, message);
    }

    int set(const Message &message, bool (*isArchivable)(const Message *msg)) const
    {
        Message *head = this->head();
        if (!isArchivable(head))
        {
            // look for next if any...
            Message *next = nullptr;
            while ((next = this->next()) && next->id != head->id && !isArchivable(next));
        }

        // IMPROVE : rank message
        // if one message is unseen by 'n' and another by 'm' where n > m
        // try to override the message that has 'm' unseen targets/clients
        int err = this->set(message);
        if(err)
        {
            std::cout << "Failed to write data to the block : " << std::endl;
        }

        return err;
    }

    int set(const unsigned int index, const Message &message) const
    {
        if (index >= m_unit)
            throw std::invalid_argument("No more space for this unit of message.");

        m_locker->lock();

        Mere::Message::Message *target = this->get(index);

        memset(target, 0, sizeof (*target));
        *target = message;

        // update the head
        // rotate the head
        m_space->head = (index + 1 == m_unit) ? 0 : index + 1;

        m_locker->free();

        return 0;
    }

    void set(const Command &command) const
    {
        m_space->command = command;
    }

    Mere::Message::Command command() const
    {
        return m_space->command;
    }

    bool ready() const
    {
        return m_ready;
    }

    void ready(bool ready)
    {
        m_ready = ready;
    }

    int numberOfUnits() const
    {
        return m_unit;
    }

private:
    int vmap()
    {
        m_space = (MessageSpace *) mmap(0, size(), PROT_READ | PROT_WRITE, MAP_SHARED, m_shm, 0);
        if (m_space == MAP_FAILED)
        {
            shm_unlink(m_name);
            Error::mmap();

            return errno;
        }

        return 0;
    }

    unsigned long long size() const
    {
        return (sizeof(unsigned int))       /* origin  */
                + sizeof (unsigned char)    /* clients */
                + sizeof (Command)
                + sizeof (unsigned char)    /* head    */
                + m_unit * m_size;
    }

private:
    char *m_name = nullptr;
    unsigned int m_unit;
    unsigned int m_size;

    int m_shm;
    bool m_ready;

    Locker *m_locker;
    MessageSpace *m_space;

    Space *q_ptr;
};

Mere::Message::Space::~Space()
{
    if (d_ptr)
    {
        delete d_ptr;
        d_ptr = nullptr;
    }
}

Mere::Message::Space::Space(const char *name, int unit, int size)
    : d_ptr(new SpacePrivate(name, unit, size, this))
{
}

int Mere::Message::Space::bind()
{
    return d_ptr->bind();
}

int Mere::Message::Space::join()
{
    return d_ptr->join();
}

int Mere::Message::Space::done()
{
    return d_ptr->done();
}

pid_t Mere::Message::Space::origin() const
{
    return d_ptr->origin();
}

Mere::Message::Message* Mere::Message::Space::prev() const
{
    return d_ptr->prev();
}

Mere::Message::Message* Mere::Message::Space::head() const
{
    return d_ptr->head();
}

Mere::Message::Message* Mere::Message::Space::next() const
{
    return d_ptr->next();
}

Mere::Message::Message* Mere::Message::Space::get() const
{
    return d_ptr->get();
}

Mere::Message::Message* Mere::Message::Space::get(unsigned int index) const
{
    return d_ptr->get(index);
}

int Mere::Message::Space::set(const Message &message) const
{
    return d_ptr->set(message);
}

int Mere::Message::Space::set(const Message &message, bool (*isArchivable)(const Message *msg)) const
{
    return d_ptr->set(message, isArchivable);
}

int Mere::Message::Space::set(const unsigned int index, const Message &message) const
{
    return d_ptr->set(index, message);
}

void Mere::Message::Space::set(const Command &command) const
{
    return d_ptr->set(command);
}

Mere::Message::Command Mere::Message::Space::command() const
{
    return d_ptr->command();
}

bool Mere::Message::Space::ready() const
{
    return d_ptr->ready();
}

void Mere::Message::Space::ready(bool ready)
{
    return d_ptr->ready(ready);
}

int Mere::Message::Space::numberOfUnits() const
{
    return d_ptr->numberOfUnits();
}
