#include "ClientHandler.hpp"

using namespace std;

ClientHandler::ClientHandler(QueueLockPointer_t QueueLock, QueuePointer_t Queue) :
    ProcessedClients(0),
    MemManager(MemoryManager<char>(PROCESS_MESSAGES_IN_MAX, MSGBUFFER_SIZE)),
    QueueLock(QueueLock),
    Queue(Queue)
{

    DBG(120, "Constructor");

    //- init clients map
    Clients.reserve(CLIENTS_MAX);

    //- setup epoll
    EpollFD = epoll_create1(0);

    //- reserve messages memory
    Messages.reserve(PROCESS_MESSAGES_IN_MAX);
}

ClientHandler::~ClientHandler()
{
    DBG(120, "Destructor");
}

void ClientHandler::addClient(const int ClientFD)
{

    //- add client fd to client map
    time_t ActualTime = time(0);
    Clients.insert(pair<int, const int>(ClientFD, ActualTime));

    //- set client connection non blocking
    Socket::makeNonblocking(ClientFD);

    //- add fd to epoll
    EpollEvent.events = EPOLLIN | EPOLLET;
    EpollEvent.data.fd = ClientFD;

    epoll_ctl(EpollFD, EPOLL_CTL_ADD, ClientFD, &EpollEvent);

}

void ClientHandler::processClients()
{

    //- reset processed clients
    ProcessedClients = 0;

    //- get epoll ready filedescriptors
    int FDCount = epoll_wait(
        EpollFD,
        EpollEvents,
        RECEIVER_PROCESS_PER_LOOP_COUNT,
        0
    );

    DBG(190, "Epoll ready FD count:" << FDCount);

    //- on error
    if (FDCount == -1) {
        ERR("Epoll Error:" << errno);
        ProcessedClients = 0;
        return;
    }

    //- if filedesctiptors to process
    if (FDCount > 0) {

        //- read client data
        readClientData(FDCount);

        //- transfer messages to queue
        transferMessages();

    }

}

inline void ClientHandler::readClientData(const int FDCount)
{

    DBG(70, "Read client data. Filedescriptor count:" << FDCount);

    //- read filedescriptor content
    for (int i=0; i<FDCount; ++i) {

        // read message into buffer
        char* MsgPointer = MemManager.getNextMemPointer();
        int RcvBytes = read(EpollEvents[i].data.fd, MsgPointer, MSGBUFFER_SIZE);

        //- client closed connection
        if (RcvBytes == 0) {
            DBG(100, "Close FD:" << EpollEvents[i].data.fd);
            close(EpollEvents[i].data.fd);
        }

        //- add message to queue
        else {

            DBG(70, "Add message to queue i:" << i);

            Messages.insert(
                Messages.begin(),
                {
                    static_cast<uint16_t>(EpollEvents[i].data.fd),
                    MsgPointer,
                    static_cast<uint16_t>(RcvBytes)
                }
            );
        }
    }

    //- set processed clients count
    ProcessedClients = FDCount;

}

inline void ClientHandler::transferMessages()
{

    //- copy element vector
    vector<MessageTransfer_t> TransferMessages(Messages);

    //- clear messages
    Messages.clear();

    DBG(100, "Transfer Messages to queue");
    DBG(100, "QueueLock:" << *QueueLock << " TransferMessages.size():" << TransferMessages.size());

    //- transfer messages if lock released
    if (*QueueLock == LOCK_STATE_PROCESSED_QUEUE) {

        DBG(100, "Putting messages inside queue");

        //- put messages to queue
        uint MessageCount = TransferMessages.size();
        for (uint i=0; i<MessageCount; ++i) {

            DBG(70, "Iterator i:" << i);

            //- add message struct refs to queue
            Queue->push(TransferMessages.back());

            //- delete last element
            TransferMessages.pop_back();

        }

        //- queue release lock
        *QueueLock = LOCK_STATE_WROTE_QUEUE;
    }

    DBG(100, "QueueLock:" << *QueueLock << " Queue.size():" << Queue->size());

}
