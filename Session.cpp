#include "Session.hpp"

using namespace std;

Session::Session()
{
    DBG(120, "Constructor");
    SessionData.reserve(CLIENTS_MAX);
}

Session::~Session()
{
    DBG(120, "Destructor");
}

void Session::add(const string& SessionID)
{
    unique_ptr<Message> Messages(new Message());
    SessionData.insert(std::make_pair(SessionID, std::move(Messages)));
}

Session::SessionData_t& Session::getSessionData()
{
    return SessionData;
}

Session::SessionDataIter_t Session::findSessionElement(const string& SessionID)
{
    return SessionData.find(SessionID);
}

void Session::addMessage(const string& SessionID, const string& Payload)
{
    Session::SessionDataIter_t SessionDataIter = findSessionElement(SessionID);
    SessionDataIter->second->addMessage(Payload);
}

Session::MessagePointer_t Session::getMessageObject(const string& SessionID)
{
    SessionDataIter_t SessionDataIter = findSessionElement(SessionID);

    if (SessionDataIter != SessionData.end()) {
        return SessionDataIter->second;
    }

    return nullptr;
}
