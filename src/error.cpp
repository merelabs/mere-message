#include "error.h"

#include <cstring>
#include <iostream>

//static
int Mere::Message::Error::shm_open()
{
    std::cerr << "Unable to open shared message space." << strerror(errno) << std::endl;

    switch (errno)
    {
        case ENOENT:
            std::cerr << "Can not connect with provided shared memory segment." << std::endl;
            break;

        case EACCES:
            std::cerr << "Insufficient priviledge, required permissions (read and write) denied." << std::endl;
            break;
    }

    return errno;
}

//static
int Mere::Message::Error::ftruncate()
{
    std::cout << "Unable to truncate shared message space" << strerror(errno) << std::endl;

    switch (errno)
    {
        case EBADF:
            std::cerr << "Shared memory file descriptor is not a valid descriptor." << std::endl;
            break;

        case EINVAL:
            std::cerr << "Shared memory file descriptor is not open for writing." << std::endl;
            break;

        case EIO:
            std::cerr << "An I/O error occurred updating the inode." << std::endl;
            break;

        case EACCES:
            std::cerr << "Insufficient priviledge, required permissions (write) denied." << std::endl;
            break;
    }

    return errno;
}

//static
int Mere::Message::Error::mmap()
{
    std::cout << "Unable to mapped shared message space into memory:" << strerror(errno) << std::endl;

    switch (errno)
    {
        case EBADF:
            std::cerr << "Shared memory file descriptor is not a valid descriptor." << std::endl;
            break;

        case EINVAL:
            std::cerr << "Shared memory file descriptor is not open for writing." << std::endl;
            break;

        case EIO:
            std::cerr << "An I/O error occurred updating the inode." << std::endl;
            break;

        case EACCES:
            std::cerr << "Insufficient priviledge, required permissions (read and write) denied." << std::endl;
            break;
    }

    return errno;
}
