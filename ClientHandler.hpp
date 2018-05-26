#ifndef ClientHandler_hpp
#define ClientHandler_hpp

#include <sys/socket.h>
#include <sys/time.h>
#include <sys/epoll.h>

#include <string.h>
#include <errno.h>
#include <fcntl.h>

#include <iostream>
#include <ctime>
#include <atomic>
#include <queue>
#include <unordered_map>

#include "Helper.hpp"
#include "Global.hpp"
#include "MemoryManager.hpp"
#include "MessageServer.hpp"


using namespace std;


class ClientHandler {

public:

    ClientHandler(QueueLockPointer_t, QueuePointer_t);
    ~ClientHandler();

    void addClient(const int);
    void processClients();

    inline void readClientData(const int);
    inline void transferMessages();

    uint8_t ProcessedClients;

private:

    MemoryManager<char> MemManager;

    QueueLockPointer_t QueueLock;
    QueuePointer_t Queue;

    unordered_map<int, const int> Clients;
    unordered_map<int, const int>::const_iterator ClientsIter;

    struct epoll_event EpollEvent, EpollEvents[RECEIVER_PROCESS_PER_LOOP_COUNT];

    vector<MessageTransfer_t> Messages;

    int EpollFD;

};

#endif
