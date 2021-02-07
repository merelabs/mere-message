#ifndef MERE_MESSAGE_MESSAGE_H
#define MERE_MESSAGE_MESSAGE_H

#include "types.h"

#include <sys/types.h>

namespace Mere
{
namespace Message
{

typedef struct
{
    /* meta block */
    mid_t id;         // id
    sid_t sid;        // session id
    unsigned char signal;     // signal
    unsigned char channel;    //
    unsigned char clients;    // number of clients

    /* route block */
    struct
    {
        pid_t source;
        pid_t target;
    } _rblock;

    /* data block  */
    struct dblock
    {
        mime_t type;

        // id, sid, signal, channel, clients, _rblock, mimetype
        char data[4096 - sizeof (mid_t)
                       - sizeof (sid_t)
                       - sizeof (unsigned char) * 3
                       - sizeof (_rblock)
                       - sizeof (mime_t)];
    } _dblock;
} Message;

enum MimeType {TEXT, JSON};

}
}

#endif // MERE_MESSAGE_MESSAGE_H
