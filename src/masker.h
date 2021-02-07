#ifndef MERE_MESSAGE_MASKER_H
#define MERE_MESSAGE_MASKER_H

#include "global.h"
#include <utility>

namespace Mere
{
namespace Message
{

class MERE_MESSAGE_LIB_SPEC Masker
{
public:
    virtual ~Masker();
    Masker();

    int mask(std::pair<uchar, uchar> input);
    std::pair<unsigned char, unsigned char> unmask(int mask);
};

}
}

#endif // MERE_MESSAGE_MASKER_H
