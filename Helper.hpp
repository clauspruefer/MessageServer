#ifndef Helper_hpp
#define Helper_hpp

#include "Debug.cpp"

#include "stdint.h"

using namespace std;

class Socket {

public:

    static void makeNonblocking(int fd)
    {
        int flags = fcntl(fd, F_GETFL, 0);

        if (fcntl(fd, F_SETFL, flags | O_NONBLOCK ) < 0) {
            ERR("Error setting Socket flag O_NONBLOCKING.");
            exit(EXIT_FAILURE);
        }

    }
};

#endif
