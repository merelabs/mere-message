#ifndef MERE_MESSAGE_SPACE_H
#define MERE_MESSAGE_SPACE_H

#include "global.h"
#include "message.h"
#include "command.h"
#include "locker.h"

#include <QObject>
#include <QDebug>

namespace Mere
{
namespace Message
{

// changing the structure require changes the
// size() emthod
typedef struct
{
    /* meta block */
    unsigned int origin;     // pid of the procss that created it
    unsigned char clients;    // number of clients

    /* command block */
    Command command;

    // message block
    unsigned char head;
    Message messages[];

} MessageSpace;

class MERE_MESSAGE_LIB_SPEC Space
{
public:
    virtual ~Space();
    explicit Space(const char *name, int unit = 1, int size = 4096);

    int bind();
    int join();
    int done();

    pid_t origin() const;

    // move head to point to the previous
    // block and return it
    Message* prev() const;

    // return block that the head pointing to
    Message* head() const;

    // move head to point to the next
    // block and return it
    Message* next() const;

    Message* get() const;
    Message* get(unsigned int index) const;

    int set(const Message &message) const;
    int set(const Message &message, bool (*isArchive)(const Message *msg)) const;
    int set(const unsigned int index, const Message &message) const;

    Command command() const;
    void set(const Command &command) const;

    bool ready() const;
    void ready(bool ready);

    int numberOfUnits() const;

private:
    class SpacePrivate;
    SpacePrivate *d_ptr;
};

}
}

#endif // MERE_MESSAGE_SPACE_H
