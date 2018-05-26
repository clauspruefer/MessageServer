#ifndef Config_hpp
#define Config_hpp

#include "string.h"

#include <iostream>
#include <string>
#include <vector>
#include <atomic>
#include <thread>

#include "Session.hpp"
#include "Message.hpp"

#include "../../Debug.cpp"
#include "../../MemoryManager.hpp"

#include "../../lib/ConnectionLocal.hpp"


using namespace std;

class Thread {

public:

    typedef atomic<bool> RunThreads_t;
    typedef shared_ptr<RunThreads_t> RunThreadsP_t;

    typedef vector<MessageItem> MessageItems_t;
    typedef shared_ptr<MessageItems_t> MessageItemsP_t;
    typedef MessageItems_t::iterator MessageItemsIter_t;

    typedef shared_ptr<Thread> ThreadP_t;

    typedef MemoryManager<char> MemoryManager_t;
    typedef shared_ptr<MemoryManager_t> MemoryManagerP_t;

    Thread(uint, RunThreadsP_t, uint, uint);
    ~Thread();

    void setConnectionType(const string);
    void setMessageType(const string);
    void setMessageLengthMin(const uint);
    void setMessageLengthMax(const uint);

    void generateMessages(Session* SessionRef);

    void spawn();
    void start();
    void waitFinished();

    static void process(ThreadP_t);

private:

    uint ThreadID;

    RunThreadsP_t startThreads;
    MemoryManagerP_t MemMessages;

    string ConnectionType;
    string MessageType;

    uint SessionAmount;
    uint MessageAmount;
    uint MessageLengthMin;
    uint MessageLengthMax;

    MessageItemsP_t MessageItems;

    thread ThreadObj;

};

#endif
