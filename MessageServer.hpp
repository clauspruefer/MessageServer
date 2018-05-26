#ifndef MsgServer_hpp
#define MsgServer_hpp

#include <sys/socket.h>
#include <sys/un.h>

#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <poll.h>

#include <string>
#include <map>
#include <thread>
#include <vector>
#include <iterator>
#include <iostream>
#include <csignal>
#include <atomic>

#include "Constant.hpp"
#include "MemoryManager.hpp"
#include "MessageHandler.hpp"
#include "ClientHandler.hpp"

using namespace std;

//- forward declare
class ClientHandler;

//- message server class
class MsgServer
{

public:

    MsgServer();
    ~MsgServer();

    void setUnixDomainSocket(const string&);

    void Init();
    void disableSignals();
    void setupSocket();
    void setupPoll();
    void dropPrivileges();
    void ServerLoop();
    void acceptClient();

    void setTerminationHandler();
    static void terminate(int);

    unique_ptr<thread> setupThreads();

    static void processMessages(QueueLockPointer_t, QueuePointer_t);

    QueueLockPointer_t QueueLockP;
    QueuePointer_t QueueP;

private:

    unique_ptr<ClientHandler> ClientHandlerObj;

    struct sockaddr_un ServerSocketStruct, ClientSocketStruct;

    string SocketPath;

    int ServerSocketFD;

    struct pollfd ServerConnFD[1];

protected:

    static bool RunServer;

};

#endif
