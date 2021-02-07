#include "masker.h"

Mere::Message::Masker::~Masker()
{

}

Mere::Message::Masker::Masker()
{

}

int Mere::Message::Masker::mask(std::pair<uchar, uchar> input)
{
    return ((unsigned short) input.first << 8 | input.second);
}

std::pair<uchar, uchar> Mere::Message::Masker::unmask(int mask)
{
    unsigned short input = mask;

    std::pair<uchar, uchar> pair;
    pair.first  = input >> 8;
    pair.second = input ^ (pair.first << 8);

    return pair;
}

