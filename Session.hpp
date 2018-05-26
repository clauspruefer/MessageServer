#ifndef Session_hpp
#define Session_hpp

#include <sys/socket.h>
#include <sys/un.h>

#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <poll.h>

#include <string>
#include <unordered_map>
#include <iterator>
#include <memory>
#include <iostream>

#include "Constant.hpp"
#include "Message.hpp"


using namespace std;

//- session class
class Session
{

public:

    Session();
    ~Session();

    typedef shared_ptr<Message> MessagePointer_t;
    typedef unordered_map<string, const MessagePointer_t> SessionData_t;
    typedef SessionData_t::const_iterator SessionDataIter_t;
    typedef shared_ptr<SessionData_t> SessionDataPointer_t;

    void add(const string&);
    void addMessage(const string&, const string&);

    MessagePointer_t getMessageObject(const string&);
    SessionDataIter_t findSessionElement(const string&);
    SessionData_t& getSessionData();


private:

    SessionData_t SessionData;

protected:


};

#endif
