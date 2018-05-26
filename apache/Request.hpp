#ifndef Request_hpp
#define Request_hpp

#include <string>
#include <vector>
#include <ctime>


#include "Main.hpp"
#include "mod_msg.hpp"
#include "../Constant.hpp"
#include "../Error.hpp"

#include "../lib/ConnectionLocal.hpp"

using namespace std;

//- forward declare Main
class Main;

//- request class
class Request {

public:

    Request(Main*, request_rec*);
    ~Request();

    int process();
    void parseURI();
    void processMessage();

private:

    Main* MainObj;

    request_rec* Req;

    string MessageType;
    string MessageSession;
    string MessagePayload;

    int ReturnCode;

};

#endif
