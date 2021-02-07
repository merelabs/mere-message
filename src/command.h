#ifndef MERE_MESSAGE_COMMAND_H
#define MERE_MESSAGE_COMMAND_H

#include "global.h"
#include <QObject>

namespace Mere
{
namespace Message
{

enum Method : unsigned char
{
    SELF,
    SEEN,
    ACKN,
    JOIN,
    PING,
    PONG,
    POST,
    BYE
};

typedef struct
{
    pid_t source;
    pid_t target;
    Method method;
    char args[16];
} Command;

}
}
#endif // MERE_MESSAGE_COMMAND_H
