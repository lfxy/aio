#include "aio.h"

#include <iostream>
#include <fcntl.h>  
//#include <sys/stat.h>
//#include <unistd.h>
////#include <stdio.h>  
#include <stdlib.h> 
#include <string.h>
#include <errno.h>
#include <stdio.h>

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

int Aio::aio_open(std::vector<char*>& multipath, int flags, mode_t mode)
{
    int rc;
    for (std::vector<char*>::iterator it = multipath.begin(); it != multipath.end(); ++it)
    {
        rc = open(*it, flags | O_NONBLOCK, mode);
        if(rc < 0)
            return rc;
        m_multiFd.push_back(rc);
    }
    return 0;
}

int Aio::aio_close(int fd)
{
    int rc;
    rc = close(fd);
    return rc;
}

int Aio::aio_close()
{
    for (std::vector<int>::iterator it = m_multiFd.begin(); it != m_multiFd.end(); ++it)
    {
        int rc;
        rc = close(*it);
        if(rc < 0)
        {
            m_multiFd.clear();
            return rc;
        }
    }
    m_multiFd.clear();
    return 0;
}
int Aio::aio_read(int fd, void* buf, ssize_t count)
{

    return 0;
}

int Aio::aio_select_read(int fd, void* buf, ssize_t count)
{
    ssize_t rc;
    rc = read(fd, buf, count);
    if (rc < 0)
    {
        if(errno == EAGAIN)
        {
            printf("aio_read_use_select start select\n");
            fd_set rset;
            FD_ZERO(&rset);
            FD_SET(fd, &rset);
            select(10, &rset, NULL, NULL, NULL);
            if (FD_ISSET(fd, &rset))
            {
                rc = read(fd, buf, count);
                printf("select retured, read %d byte\n%s\n", (int)rc, (char*)buf);
            }
        }
        else
        {
            printf("unkown error\n");
            exit(-1);
        }
    }
    else
    {
        printf("read %d byte\n%s\n", (int)rc, (char*)buf);
    }
    

    return rc;
}

int Aio::aio_select_read()
{
    ssize_t rc;
    fd_set rset;
    FD_ZERO(&rset);
    /*for (std::vector<int>::iterator it = m_multiFd.begin(); it != m_multiFd.end(); ++it)
    {
        //rc = read(fd, buf, count);
        FD_SET(*it, &rset);
    }
    select(10, &rset, NULL, NULL, NULL);
    bool bleft = false;
    for (std::vector<int>::iterator it = m_multiFd.begin(); it != m_multiFd.end(); ++it)
    {
        if (FD_ISSET(*it, &rset))
        {
            rc = read(*it, buf, count);
            printf("select retured, read %d byte\n%s\n", (int)rc, (char*)buf);
        }
        else
        {
            bleft = true;
            vleftfd.push_back(*it);
        }
    }*/
    std::vector<int> vleftfd = m_multiFd;
    while(!vleftfd.empty())
    {
        for (std::vector<int>::iterator it = vleftfd.begin(); it != vleftfd.end(); ++it)
        {
            FD_SET(*it, &rset);
        }
        select(10, &rset, NULL, NULL, NULL);
        for (std::vector<int>::iterator it = vleftfd.begin(); it != vleftfd.end();)
        {
            if (FD_ISSET(*it, &rset))
            {
                char buf[10000] = {0};
                ssize_t count = 1000;
                rc = read(*it, buf, count);
                it = vleftfd.erase(it);
                printf("select retured, read %d byte\n%s\n", (int)rc, (char*)buf);
            }
            else
            {
                ++it;
            }
        }
    }
    
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
    ssize_t rc;
    while(1)
    {
        rc = read(fd, buf, count);
        if (rc < 0)
        {
            if(errno == EAGAIN)
            {
                printf("aio read again after 1s\n");
                sleep(1);
            }
            else
            {
                printf("unkown error\n");
                exit(-1);
            }
        }
        else
        {
            printf("read %d byte\n%s\n", (int)rc, (char*)buf);
            break;
        }
    }
    /*fd_set rset;
    FD_ZERO(&rset);
    FD_SET(fd, &rset);*/

    return rc;
}
