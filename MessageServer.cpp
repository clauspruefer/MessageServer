#include "MessageServer.hpp"

using namespace std;

bool MsgServer::RunServer = true;

MsgServer::MsgServer() :
    QueueLockP(new QueueLock_t(LOCK_STATE_PROCESSED_QUEUE)),
    QueueP(new Queue_t())
{
    DBG(120, "Constructor");
    ClientHandlerObj = static_cast<unique_ptr<ClientHandler>>(new ClientHandler(QueueLockP, QueueP));
}

MsgServer::~MsgServer()
{
    DBG(120, "Destructor");
}

void MsgServer::Init()
{

    //- disable OS signals SIGINT, SIGPIPE
    disableSignals();

    //- setup server socket
    setupSocket();

    //- setup server socket monitoring
    setupPoll();

    //- setup termination handler
    setTerminationHandler();

}

void MsgServer::setUnixDomainSocket(const string& UnixPath)
{
    SocketPath = UnixPath;
}

void MsgServer::disableSignals()
{
    //- disable unwanted signal handlers (SIGINT, SIGPIPE)
    signal(SIGINT,  SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
}

void MsgServer::setTerminationHandler()
{
    signal(SIGTERM, MsgServer::terminate);
}

void MsgServer::terminate(int _ignored)
{
    DBG(-1, "SIGTERM received, shutting down");
    RunServer = false;
}

void MsgServer::dropPrivileges()
{

    //- in case of being root, drop privileges
    if (getuid() == 0) {

        if (setgid(RUN_GROUP_ID) != 0) {
            cout << "Failed setting groupid to:" << RUN_GROUP_ID << endl;
            exit(EXIT_FAILURE);
        }

        if (setuid(RUN_USER_ID) != 0) {
            cout << "Failed setting userid to:" << RUN_USER_ID << endl;
            exit(EXIT_FAILURE);
        }

    }
    else {
        cout << "Error. Running server as root user is recommended." << endl;
        exit(EXIT_FAILURE);
    }
}

void MsgServer::setupSocket()
{

    DBG(60, "try setting up Unix Domain Socket Server.");

    //- try setup server socket
    try
    {

        //- unlink socket fifo
        unlink(SocketPath.c_str());

        //- setup socket
        ServerSocketFD = socket(AF_UNIX, SOCK_SEQPACKET, 0);
        DBG(80, "Server SocketFD:" << ServerSocketFD);

        ServerSocketStruct.sun_family = AF_UNIX;
        strcpy(ServerSocketStruct.sun_path, SocketPath.c_str());

        //- make server socket nonblocking
        Socket::makeNonblocking(ServerSocketFD);

        //- bind socket
        int rc;
        rc = bind(
            ServerSocketFD,
            reinterpret_cast<struct sockaddr*>(&ServerSocketStruct),
            sizeof(ServerSocketStruct)
        );

        if (rc == -1) {
            ERR("Server Socket bind() Error:" << errno << " ErrorString:" << strerror(errno));
            exit(1);
        }

        //- listen
        rc = listen(ServerSocketFD, NET_BACKLOG);

        if (rc == -1) {
            ERR("Server Socket listen() Error:" << errno << " ErrorString:" << strerror(errno));
            exit(1);
        }

    }
    catch( const char* msg )
    {
        ERR("Setup SocketFD failed:" << msg);
        exit(1);
    }

}

void MsgServer::setupPoll()
{
    ServerConnFD[0].fd = ServerSocketFD;
    ServerConnFD[0].events = POLLIN;
}

unique_ptr<thread> MsgServer::setupThreads()
{
    DBG(90, "Setup worker thread(s)");

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(2, &cpuset);

    thread* ProcessMessageThread = new thread(processMessages, QueueLockP, QueueP);

    int rc = pthread_setaffinity_np(
        ProcessMessageThread->native_handle(),
        sizeof(cpu_set_t),
        &cpuset
    );

    if (rc == -1) {
        ERR("Error setting cpu affinity.");
        exit(1);
    }

    return unique_ptr<thread>(ProcessMessageThread);
}

void MsgServer::ServerLoop()
{

    //- setup network processing thread (1) and worker thread (1)
    unique_ptr<thread> t(setupThreads());

    DBG(-1, "Network processing thread running on CPU:" << sched_getcpu());

    //- main server loop
    while(RunServer) {

        //- poll server fd for incoming connections
        int rc;

        DBG(190, "Server loop, poll server fd for incoming connections");
        rc = poll(ServerConnFD, 1, 0);

        if (rc == -1) {
            ERR("Server Socket poll Error:" << errno << " ErrorString:" << strerror(errno));
        }
        else {

            DBG(190, "Server poll revents:" << ServerConnFD[0].revents);

            //- check for incoming connection
            if (ServerConnFD[0].revents & POLLIN) {
                acceptClient();
            }
            //- no new client and no processed clients (idle)
            else if (ClientHandlerObj->ProcessedClients == 0) {
                this_thread::sleep_for(chrono::nanoseconds(1));
            }

        }

        //- process clients
        ClientHandlerObj->processClients();
    }

    //- join threads
    t->join();

    DBG(10, "Server exit");

}

void MsgServer::acceptClient()
{

    socklen_t ClientSocketLen;
    ClientSocketLen = sizeof(ClientSocketStruct);

    int ClientFD = accept(
        ServerSocketFD,
        reinterpret_cast<struct sockaddr*>(&ClientSocketStruct),
        &ClientSocketLen
    );

    if (ClientFD > 0) {
        DBG(80, "Accepted client fd:" << ClientFD);
        ClientHandlerObj->addClient(ClientFD);
    }
    else {
        ERR("Accept client fd failed");
    }

}

void MsgServer::processMessages(QueueLockPointer_t QueueLockP, QueuePointer_t QueueP) {

    unique_ptr<MessageHandler> MsgHandlerRef(new MessageHandler());

    DBG(-1, "Messaging processing thread running on CPU:" << sched_getcpu());

    while(RunServer) {
        MsgHandlerRef->processMessages(QueueLockP, QueueP);
    }

}
