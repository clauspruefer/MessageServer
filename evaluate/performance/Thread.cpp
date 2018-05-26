#include "Thread.hpp"

using namespace std;

Thread::Thread(uint id, RunThreadsP_t ThreadsRun, uint SessionAmount, uint MessageAmount):
    ThreadID(id),
    startThreads(ThreadsRun),
    MemMessages(new MemoryManager<char>(SessionAmount*(MessageAmount+1), MSGBUFFER_SIZE)),
    MessageType("SET"),
    SessionAmount(SessionAmount),
    MessageAmount(MessageAmount),
    MessageLengthMin(0),
    MessageLengthMax(0),
    MessageItems(new MessageItems_t())
{
    DBG(95, "Constructor");
}

Thread::~Thread()
{
    DBG(95, "Destructor");
}

void Thread::setConnectionType(const string Type)
{
    ConnectionType = Type;
}

void Thread::setMessageType(const string Type)
{
    MessageType = Type;
}

void Thread::setMessageLengthMin(const uint Length)
{
    MessageLengthMin = Length;
}

void Thread::setMessageLengthMax(const uint Length)
{
    MessageLengthMax = Length;
}

void Thread::generateMessages(Session* SessionRef)
{

    for (uint x=0; x<SessionAmount; ++x) {

        string SessionID = SessionRef->generateID();
        DBG(95, "SessionID:" << SessionID);

        for (uint y=0; y<MessageAmount; ++y) {

            uint MessageLength;

            MessageLength = (MessageLengthMin == MessageLengthMax) ? MessageLengthMin : Random::generateRandIntRange(MessageLengthMin, MessageLengthMax);

            DBG(95, "MessageLengthMin:" << MessageLengthMin << " MessageLengthMax:" << MessageLengthMax);

            Message Msg;
            Msg.setType(MessageType);
            Msg.setSessionID(SessionID);
            Msg.setPayloadLength(MessageLength);
            Msg.generateRandomPayload();

            MessageItem MsgItem;
            string Message = Msg.getMessage();

            MsgItem.MessageLength = Message.length();

            //- memory managed buffer destination address
            char* DestinationAddress = MemMessages->getNextMemPointer();

            //- copy generated message to buffer address
            memcpy(
                DestinationAddress,
                static_cast<const void*>(Message.c_str()),
                MsgItem.MessageLength
            );

            DBG(95, "Message:" << Message << " Length:" << MsgItem.MessageLength << " Address:" << static_cast<void*>(DestinationAddress));

            MsgItem.MemPointer = DestinationAddress;

            MessageItems->push_back(MsgItem);

        }

    }

}

void Thread::spawn()
{
    ThreadObj = thread(Thread::process, ThreadP_t(this));
}

void Thread::waitFinished()
{
    ThreadObj.join();
}

void Thread::process(ThreadP_t t)
{

    DBG(-1, "MemMessages Object Address:" << t->MemMessages);

    unique_ptr<ConnectionLocal> ConnectionObj(new ConnectionLocal());

    DBG(95, "Connection Type:" << t->ConnectionType);

    ConnectionObj->setType(t->ConnectionType);
    ConnectionObj->setUnixDomainSocketName(SERVER_SOCKET);

    if (t->ConnectionType == "persistent") {
        ConnectionObj->connect();
    }

    //- wait for thread start
    while(!*t->startThreads) { this_thread::yield(); }

    uint MessageCount = t->MessageItems->size();

    DBG(-1, "Thread (" << t->ThreadID << ") start sending messages (" << MessageCount << ") on cpu:" << sched_getcpu());
    DBG(95, "Connection Type:" << t->ConnectionType);

    string Result;

    for (uint i=0; i<MessageCount; ++i) {

        /*
        if (ConnectionObj->ConnectionType == "nonpersistent") {
            ConnectionObj->connect();
        }
        */

        DBG(70, "Sending msg(" << i << ") with memory address:" << static_cast<void*>(t->MessageItems->at(i).MemPointer) << " length:" << t->MessageItems->at(i).MessageLength);

        string tmp(t->MessageItems->at(i).MemPointer, t->MessageItems->at(i).MessageLength);
        //DBG(95, "Message payload:" << tmp);

        ConnectionObj->sendMessage(t->MessageItems->at(i).MemPointer, t->MessageItems->at(i).MessageLength);

        //DBG(70, "Try receiving result");
        ConnectionObj->receiveMessage(Result);
        //DBG(70, "Received result (" << i << "):" << Result);

        /*
        if (ConnectionObj->ConnectionType == "nonpersistent") {
            ConnectionObj->close();
        }
        */

        //this_thread::sleep_for(chrono::seconds(1));

    }

}
