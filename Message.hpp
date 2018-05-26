#ifndef Message_hpp
#define Message_hpp

#include <sys/socket.h>
#include <sys/un.h>

#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <poll.h>

#include <string>
#include <vector>
#include <iterator>
#include <memory>
#include <iostream>

#include "Debug.cpp"
#include "Constant.hpp"


using namespace std;

//- message server class
class Message
{

public:

    Message();
    ~Message();

    typedef struct MessageItem {
        string Message;
        time_t MessageTime;
    } MessageItem_t;

    typedef vector<MessageItem_t> Messages_t;

    void addMessage(const string&);
    Messages_t& getMessages();

private:

    Messages_t Messages;

protected:


};

#endif
