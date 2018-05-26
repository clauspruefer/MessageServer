#include "MessageHandler.hpp"

using namespace std;

MessageHandler::MessageHandler()
{
    DBG(120, "Constructor");
    QueueMessages.reserve(PROCESS_MESSAGES_IN_MAX);
}

MessageHandler::~MessageHandler()
{
    DBG(120, "Destructor");
}

void MessageHandler::processMessages(QueueLockPointer_t QueueLock, QueuePointer_t Queue)
{

    //- check lock status
    if (*QueueLock == LOCK_STATE_WROTE_QUEUE) {

        uint QueueSize = Queue->size();

        DBG(100, "Queue.size():" << QueueSize << " QueueMessages.size():" << QueueMessages.size());

        //- put messages from queue to processing messages
        for (uint i=0; i<QueueSize; ++i) {
            QueueMessages.push_back(Queue->front());
            Queue->pop();
        }

        DBG(100, "Releasing Queue lock");

        //- queue release lock
        *QueueLock = LOCK_STATE_PROCESSED_QUEUE;

    }

    //- relax processor
    if (QueueMessages.size() == 0) {
        this_thread::sleep_for(chrono::nanoseconds(1000));
        return;
    }

    //- dispatch messages
    uint MsgCount = (QueueMessages.size() > MSGPROCESSOR_PROCESS_PER_LOOP_COUNT) ? MSGPROCESSOR_PROCESS_PER_LOOP_COUNT : QueueMessages.size();

    DBG(100, "Dispatch Messages count:" << MsgCount << " QueueMessages.size():" << QueueMessages.size());

    vector<uint> DeleteMessages;

    for (uint i=0; i<MsgCount; ++i) {
        if (dispatchMessage(QueueMessages.at(i))) {
            DeleteMessages.push_back(i);
        }
    }

    //- delete processed messages
    QueueMessages.erase(DeleteMessages);

}

bool MessageHandler::dispatchMessage(MessageTransfer_t& Message)
{

    //- get message type
    string MsgType(Message.MsgBufferPointer, 3);

    //- type missmatch
    if (MsgType != MSG_INDICATOR_GET && MsgType != MSG_INDICATOR_SET) {
        DBG(50, "Message Type missmatch.");
        return true;
    }

    //- get session id
    string SessionID(Message.MsgBufferPointer+3, SESSION_ID_BYTES);

    DBG(100, "Dispatching Message Type:" << MsgType << " Session:" << SessionID);

    //- process set message type
    if (MsgType == MSG_INDICATOR_SET) {

        DBG(100, "Processing Message Type SET");

        //- if session map full, send error
        if (SessionObj.getSessionData().size() == CLIENTS_MAX) {
            DBG(100, "Session messages full");
            resultCode(E__SESSION_CONTAINER_FULL, Message.ClientFD);
            return true;
        }

        uint16_t MsgBytes = Message.MsgBytes-(SESSION_ID_BYTES+3);

        //- if message size 0, ignore msg
        if (MsgBytes == 0) {
            DBG(100, "Message Size == 0");
            return true;
        }

        string MsgPayload(Message.MsgBufferPointer+3+SESSION_ID_BYTES, MsgBytes);
        DBG(100, "Msg Payload:'" << MsgPayload << "' Bytes:" << Message.MsgBytes);

        //- if session does not exist, create
        Session::SessionDataIter_t it = SessionObj.findSessionElement(SessionID);
        if (it == SessionObj.getSessionData().end()) {
            DBG(100, "Add SessionID");
            SessionObj.add(SessionID);
        }

        //- add message
        SessionObj.addMessage(SessionID, MsgPayload);

        //- send
        resultCode(I__INSERT_MSG_OK, Message.ClientFD);

        return true;
    }

    //- process get message type
    if (MsgType == MSG_INDICATOR_GET) {

        Session::MessagePointer_t MessageObj;
        MessageObj = SessionObj.getMessageObject(SessionID);

        //- if session does not exist, retry until session exists
        if (MessageObj == nullptr) {
            DBG(100, "Session does not exist");
            return false;
        }

        //- if no messages for session, retry until messages exist
        if (MessageObj->getMessages().size() == 0) {
            DBG(100, "No Message for given Session");
            return false;
        }

        //- if session exists and send failed, remove, do not retry
        if (!sendMessages(MessageObj->getMessages(), Message.ClientFD)) {
            DBG(100, "Reply Message send failed");
            return true;
        }

        //- if send succesful, clear messages vector
        DBG(50, "Clear Messages vector");
        MessageObj->getMessages().clear();

        //- close fd
        close(Message.ClientFD);

        return true;
    }

    return false;
}

bool MessageHandler::sendMessages(Message::Messages_t& Messages, uint16_t& fd)
{
    stringstream JSONResult;

    JSONResult << "{" << endl;
    JSONResult << "\t\"Messages\":" << endl;
    JSONResult << "\t{" << endl;

    for (uint i=0; i<Messages.size(); ++i) {
        JSONResult << "\t\t\"" << (i+1) << "\": \"" << Messages.at(i).Message << "\"," << endl;
    }

    if (Messages.size() > 0) {
        JSONResult.seekp(-2, JSONResult.cur);
    }

    JSONResult << endl << "\t}" << endl;
    JSONResult << "}" << "\n\r\n\r";

    return sendReply(fd, JSONResult);
}

bool MessageHandler::sendReply(uint16_t& fd, stringstream& Payload)
{

    DBG(140, "Send fd:" << fd << " Message:" << Payload.str());

    int rc = ::write(fd, Payload.str().c_str(), Payload.str().length());

    if (rc == -1) {
        DBG(180, "Reply send error:" << strerror(errno));
    }

    DBG(140, "Send rc:" << rc);
    return (rc > 0) ? true : false;
}

inline void MessageHandler::resultCode(const int Code, uint16_t& fd)
{
    stringstream ReplyMsg;
    ReplyMsg << "{ \"ResultCode\": " << Code << " }";
    sendReply(fd, ReplyMsg);
}
