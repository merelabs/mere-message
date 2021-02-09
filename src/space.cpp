#include "space.h"
#include "error.h"

#include <iostream>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

Mere::Message::Space::~Space()
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

Mere::Message::Space::Space(const char *name, int unit, int size)
    : m_unit(unit),
      m_size(size)
{
    m_name = (char *) malloc(strlen(name) + 1);
    strcpy(m_name, name);

    m_locker = new Locker(m_name);
}

int Mere::Message::Space::bind()
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

int Mere::Message::Space::join()
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

int Mere::Message::Space::vmap()
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

int Mere::Message::Space::done()
{
    if(!ready()) return 1;

    munmap(m_space, size());
    close(m_shm);
    shm_unlink(m_name);

    ready(false);

    return 0;
}

pid_t Mere::Message::Space::origin() const
{
    return m_space->origin;
}

Mere::Message::Message* Mere::Message::Space::prev() const
{
    m_space->head = (m_space->head - 1 + m_unit) % m_unit;

    return get();
}

Mere::Message::Message* Mere::Message::Space::head() const
{
    return get();
}

Mere::Message::Message* Mere::Message::Space::next() const
{
    m_space->head = (m_space->head + 1) % m_unit;

    return get();
}

Mere::Message::Message* Mere::Message::Space::get() const
{
    return get(m_space->head);
}

Mere::Message::Message* Mere::Message::Space::get(unsigned int index) const
{
    if (index >= m_unit)
        throw std::invalid_argument("No more space for this unit of message.");

//    return m_space->messages + index;
    return &m_space->messages[index];
}

int Mere::Message::Space::set(const Message &message) const
{
    return this->set(m_space->head, message);
}

int Mere::Message::Space::set(const Message &message, bool (*isArchivable)(const Message *msg)) const
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

int Mere::Message::Space::set(const unsigned int index, const Message &message) const
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

void Mere::Message::Space::set(const Command &command) const
{
    m_space->command = command;
}

Mere::Message::Command Mere::Message::Space::command() const
{
    return m_space->command;
}

unsigned long long Mere::Message::Space::size() const
{
    return (sizeof(unsigned int))       /* origin  */
            + sizeof (unsigned char)    /* clients */
            + sizeof (Command)
            + sizeof (unsigned char)    /* head    */
            + m_unit * m_size;
}

bool Mere::Message::Space::ready() const
{
    return m_ready;
}

void Mere::Message::Space::ready(bool ready)
{
    m_ready = ready;
}

int Mere::Message::Space::numberOfUnits() const
{
    return m_unit;
}
