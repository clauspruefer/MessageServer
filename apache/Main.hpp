#ifndef Main_hpp
#define Main_hpp

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>

#include <ctime>
#include <iostream>
#include <string>

#include "mod_msg.hpp"

#include "Request.hpp"

#include "../lib/ConnectionLocal.hpp"

using namespace std;

//- forward declare Request
class Request;

//- main class
class Main {

public:

    Main(server_rec*);
    ~Main();

    ConnectionLocal ConnSetObj;
    server_rec* ServerRec;

    int run(request_rec*);

 private:

};

#endif
