#include "aio.h"

#include <iostream>
#include <fcntl.h>  
//#include <sys/stat.h>
//#include <unistd.h>
////#include <stdio.h>  
//#include <stdlib.h> 



Aio::Aio()
{
}

Aio::~Aio()
{
}

int Aio::aio_open(const char* path, int flags, mode_t mode)
{
    int rc;
    rc = open(path, flags | O_NONBLOCK, mode);
    return rc;
}

int Aio::aio_close(int fd)
{
    int rc;
    rc = close(fd);
    return rc;
}

int Aio::aio_read(int fd, void* buf, ssize_t count)
{

    return 0;
}

int Aio::aio_write(int fd, void* buf, ssize_t count)
{

    return 0;
}


int Aio::sio_unblock_open(const char* path)
{

    return 0;
}
int Aio::sio_unblock_read(int fd, void* buf, ssize_t count)
{

    return 0;
}
