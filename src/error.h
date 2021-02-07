#ifndef ERROR_H
#define ERROR_H

#include "global.h"
#include <cstring>

namespace Mere
{

namespace Message
{

class MERE_MESSAGE_LIB_SPEC Error final
{
public:
    static int shm_open();
    static int ftruncate();
    static int mmap();
};


}
}
#endif // ERROR_H
