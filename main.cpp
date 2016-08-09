
#include "aio.h"
#include <fcntl.h>  


int main()
{
    Aio aio;
    //char buf[10000] = {0};
    //int fd = -1;
    std::vector<char*> pathv;
    pathv.push_back("/dev/tty");
    pathv.push_back("./t.log");
    aio.aio_open(pathv, O_RDONLY);
    aio.aio_select_read();
    aio.aio_close();

    return 0;
}
