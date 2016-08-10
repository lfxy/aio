#ifndef _AIO_H_
#define _AIO_H_

#include <sys/types.h>  
#include <vector>

class Aio
{
    public:
        Aio();
        ~Aio();
        
        int my_aio_open(const char* path, int flags, mode_t mode = 0);
        int my_aio_close(int fd);
        int my_aio_select_read(int fd, void* buf, ssize_t count);

        int my_aio_open(std::vector<char*>& multipath, int flags, mode_t mode = 0);
        int my_aio_close();
        int my_aio_select_read();

        int my_aio_read_single_test(int fd, void* buf, ssize_t count);
        int my_aio_read_list_test();
        int my_aio_write(int fd, void* buf, ssize_t count);

        int sio_unblock_open(const char* path);
        int sio_unblock_read(int fd, void* buf, ssize_t count);

    private:
        int m_fd;
        std::vector<int> m_multiFd;
};

#endif
