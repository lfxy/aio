#include "my_aio.h"
#include <fcntl.h>  


int main()
{
    Aio aio;
    char buf[10000] = {0};
    int fd = -1;
    /*std::vector<char*> pathv;
    pathv.push_back("/dev/tty");
    pathv.push_back("./t.log");
    aio.my_aio_open(pathv, O_RDONLY);
    aio.my_aio_select_read();
    aio.my_aio_close();*/

    /*fd = aio.my_aio_open("./t.log", O_RDONLY);
    aio.my_aio_read_single_test(fd, buf, 10000);
    aio.my_aio_close();*/
    std::vector<char*> pathv;
    pathv.push_back("./lsof.txt");
    pathv.push_back("./t.log");
    aio.my_aio_open(pathv, O_RDONLY);
    aio.my_aio_read_list_test();
    aio.my_aio_close();
    return 0;
}
