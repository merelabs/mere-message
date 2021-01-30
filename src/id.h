#ifndef MERE_MESSAGE_ID_H
#define MERE_MESSAGE_ID_H

#include "global.h"
#include "types.h"

namespace Mere
{
namespace Message
{

class MERE_MESSAGE_LIB_SPEC IDChecker
{
public:
    virtual bool exists(const mid_t id) const = 0;
};

class ID
{
public:
    static mid_t next(const IDChecker &checker);
};

}
}

#endif // MERE_MESSAGE_ID_H
