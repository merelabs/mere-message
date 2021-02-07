#include "utils.h"

#include <cstring>
#include <iostream>

//static
bool Mere::Message::Utils::isValidName(const char *name)
{
    // check if starts with /
    if (name[0] != '/')
        return false;

    // check if ir cross NAME_MAX (255)
    if(strlen(name) > 255)
        return false;

    // check if there is any space
    while( *name++ != 0 )
    {
        if( *name == 32 ) return false;
    }

    return true;
}

//static
void Mere::Message::Utils::dump(const Message *message)
{
    qDebug() << "Message (" << message << ")";
//    qDebug() << "Id\t\t:" << message->id;
//    qDebug() << "Session\t\t:" << message->sid;
//    qDebug() << "Data\t\t:" << message->_dblock.data;
}
