#ifndef _AIO_H_
#define _AIO_H_

#include <sys/types.h>  
#include <vector>

class Aio
{
    public:
        Aio();
        ~Aio();
        
        int aio_open(const char* path, int flags, mode_t mode = 0);
        int aio_close(int fd);
        int aio_select_read(int fd, void* buf, ssize_t count);

        int aio_open(std::vector<char*>& multipath, int flags, mode_t mode = 0);
        int aio_close();
        int aio_select_read();

        int aio_read(int fd, void* buf, ssize_t count);
        int aio_write(int fd, void* buf, ssize_t count);

        int sio_unblock_open(const char* path);
        int sio_unblock_read(int fd, void* buf, ssize_t count);

    private:
        int m_fd;
        std::vector<int> m_multiFd;
};

#endif
