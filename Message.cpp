#include "Message.hpp"

using namespace std;

Message::Message()
{
    DBG(120, "Constructor");
    Messages.reserve(MSG_PER_SESSION_MAX);
}

Message::~Message()
{
    DBG(120, "Destructor");
}

void Message::addMessage(const string& Payload)
{
    Messages.insert(
        Messages.begin(), { string(Payload), time(0) }
    );
}

Message::Messages_t& Message::getMessages()
{    
    return Messages;
}
