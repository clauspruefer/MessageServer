#ifndef Message_hpp
#define Message_hpp

#include "../../Constant.hpp"
#include "Helper.hpp"

#include <string>
#include <iostream>
#include <vector>

typedef struct MessageItem {
  char* MemPointer;
  uint16_t MessageLength;
} MessageItem;

typedef vector<uint8_t> MessageVec_t;
typedef vector<uint8_t>::iterator MessageVecIter_t;

using namespace std;

class Message {

public:

    Message();
    ~Message();

    void setType(const string);
    void setSessionID(const string);
    void setPayloadLength(const uint);
    void setPayload(const string);
    void generateRandomPayload();
    string getMessage();

private:

    void MessageVectorToMessage(MessageVec_t*);

    string MessageType;
    string SessionID;
    string MessagePayload;

    int PayloadLength;

};

#endif
