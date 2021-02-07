#ifndef MERE_MESSAGE_UTILS_H
#define MERE_MESSAGE_UTILS_H

#include "global.h"
#include "message.h"
#include <QDebug>

namespace Mere
{
namespace Message
{

class MERE_MESSAGE_LIB_SPEC Utils final
{
private:
    Utils();

public:    
    static bool isValidName(const char *name);
    static void dump(const Message *message);
};

}
}

#endif // MERE_MESSAGE_UTILS_H
