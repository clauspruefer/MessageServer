#include "Message.hpp"

Message::Message()
{
    DBG(95, "Constructor");
    PayloadLength = 0;
}

Message::~Message()
{
    DBG(95, "Destructor");
}

void Message::setType(string Type)
{
    MessageType = Type;
}

void Message::setSessionID(string ID)
{
    SessionID = ID;
}

void Message::setPayloadLength(const uint Length)
{
    PayloadLength = Length;
}

void Message::setPayload(string Payload)
{
    MessagePayload = Payload;
}

void Message::generateRandomPayload()
{

    MessageVec_t MessageVector (PayloadLength);
    Generator::generate(MessageVector.begin(), MessageVector.end(), Random::generateRandUint8Ascii);
    MessageVectorToMessage(&MessageVector);

}

string Message::getMessage()
{

    stringstream Message;
    Message << MessageType << SessionID << MessagePayload;
    return Message.str();

}

void Message::MessageVectorToMessage(MessageVec_t* MessageVector)
{

    for (MessageVecIter_t it=MessageVector->begin(); it != MessageVector->end(); ++it) {
        char VectorValue = *it;
        MessagePayload += VectorValue;
    }

}
