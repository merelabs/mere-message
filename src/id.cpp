#include "id.h"

#include <stdlib.h>

//static
mid_t Mere::Message::ID::next(const IDChecker &checker)
{
    mid_t next = 0;
    while((next = (rand() % 120 + 1)))
    {
        if(!checker.exists(next))
            break;
    }

    return next;
}
