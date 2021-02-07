#ifndef LOCKER_H
#define LOCKER_H

#include <semaphore.h>

#include <QObject>

class Locker : public QObject
{
    Q_OBJECT
public:
    explicit Locker(const char *name, QObject *parent = nullptr);

    int bind();
    //int join();
    int done();

    bool lock();
    bool free();

private:
    void up();
signals:

private:
    const char *m_name;

    bool m_lock;
    bool m_drty;

    sem_t *m_sem;
};

#endif // LOCKER_H
