#ifndef MessageHandler_hpp
#define MessageHandler_hpp

#include <sys/timerfd.h>

#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <string>
#include <iostream>
#include <thread>

#include "Constant.hpp"
#include "Error.hpp"
#include "Debug.cpp"
#include "Global.hpp"

#include "Session.hpp"
#include "Vector.hpp"


using namespace std;


class MessageHandler {

public:

    MessageHandler();
    ~MessageHandler();

    typedef vector<MessageTransfer_t>::const_iterator QueueMessagesIter_t;

    void processMessages(QueueLockPointer_t, QueuePointer_t);
    bool dispatchMessage(MessageTransfer_t&);
    bool sendMessages(Message::Messages_t&, uint16_t&);
    bool sendReply(uint16_t&, stringstream&);
    inline void resultCode(const int, uint16_t&);

private:

    Vector<MessageTransfer_t> QueueMessages;

    Session SessionObj;
};

#endif
