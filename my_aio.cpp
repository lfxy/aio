#include "my_aio.h"

#include <iostream>
#include <fcntl.h>  
//#include <sys/stat.h>
//#include <unistd.h>
////#include <stdio.h>  
#include <stdlib.h> 
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <aio.h>

#define BUFSIZE 1000
#define MAX_LIST 10

Aio::Aio()
{
}

Aio::~Aio()
{
}

int Aio::my_aio_open(const char* path, int flags, mode_t mode)
{
    int rc;
    rc = open(path, flags | O_NONBLOCK, mode);
    if (rc < 0) 
        perror("open");
    return rc;
}

int Aio::my_aio_open(std::vector<char*>& multipath, int flags, mode_t mode)
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

int Aio::my_aio_close(int fd)
{
    int rc;
    rc = close(fd);
    return rc;
}

int Aio::my_aio_close()
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
int Aio::my_aio_read_single_test(int fd, void* buf, ssize_t count)
{
    if (!buf && fd < 0) 
    {
        perror("param error\n");
        return -1;
    }
    int rc;
    struct aiocb my_aiocb;

    bzero( (char *)&my_aiocb, sizeof(struct aiocb) );

    my_aiocb.aio_buf = buf;

    /* Initialize the necessary fields in the aiocb */
    my_aiocb.aio_fildes = fd;
    my_aiocb.aio_nbytes = count;
    my_aiocb.aio_offset = 0;

    rc = aio_read( &my_aiocb );
    if (rc < 0) perror("aio_read");

    /*while(aio_error( &my_aiocb ) == EINPROGRESS);
    if ((rc = aio_return( &my_aiocb )) > 0) 
        printf("select retured, read %d byte\n%s\n", (int)rc, (char*)my_aiocb.aio_buf);
    else 
        printf("read failed, rc:%d, errno:%d\n", (int)rc, errno);
        */
    struct aiocb *cblist[MAX_LIST];
    bzero( (char *)cblist, sizeof(cblist) );
    cblist[0] = &my_aiocb;
    rc = aio_suspend(cblist, MAX_LIST, NULL);
    if(rc < 0)
        printf("read failed, rc:%d, errno:%d\n", (int)rc, errno);
    else
        printf("select retured, read %d byte\n%s\n", (int)rc, (char*)my_aiocb.aio_buf);

    return 0;
}

int Aio::my_aio_read_list_test()
{
    int rc = -1;
    int list_size = m_multiFd.size();
    struct aiocb* list[list_size];
    bzero((char*)list, sizeof(list));
    //for (std::vector<int>::iterator it = m_multiFd.begin(); it != m_multiFd.end(); ++it)
    for (unsigned int i = 0; i < list_size; ++i)
    {
        aiocb* paiocb = new aiocb;
        paiocb->aio_fildes = m_multiFd[i];
        paiocb->aio_buf = new char[BUFSIZE+1];
        paiocb->aio_nbytes = BUFSIZE;
        paiocb->aio_offset = 0;
        paiocb->aio_lio_opcode = LIO_READ;
        list[i] = paiocb;
    }

    rc = lio_listio(LIO_WAIT, list, 2, NULL);

    for (unsigned int i = 0; i < list_size; ++i)
    {
        printf("--------------\nretured, read %d byte\n%s\n", (int)list[i]->aio_nbytes, (char*)list[i]->aio_buf);
        delete[] (char*)list[i]->aio_buf;
        delete list[i];
    }

    return rc;
}

int Aio::my_aio_select_read(int fd, void* buf, ssize_t count)
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

int Aio::my_aio_select_read()
{
    ssize_t rc;
    fd_set rset;
    FD_ZERO(&rset);
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

int Aio::my_aio_write(int fd, void* buf, ssize_t count)
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

    return rc;
}
