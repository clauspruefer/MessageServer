#ifndef Connection_hpp
#define Connection_hpp

#include "../Debug.cpp"

#include <stdio.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/un.h>

#include <iostream>
#include <string>
#include <thread>

using namespace std;

class Connection {

public:

    Connection();
    ~Connection();

    string ConnectionType;

    bool ConnStatus;
    bool SendStatus;
    bool RecvStatus;

    int SocketFD;
    int RecvBytes;

    virtual void close();
    virtual void setType(const string);

protected:

    virtual void connect() = 0;
    virtual void reconnect() = 0;
    virtual void sendMessageReconnect(const stringstream&) = 0;
    virtual void sendMessage(const stringstream&) = 0;
    virtual void sendMessage(const char*, const int&) = 0;
    virtual void receiveMessage(string&) = 0;
    virtual void setSendStatus(int) = 0;

    char RecvBuffer[MSGBUFFER_SIZE];

    sockaddr_un SocketStruct;
    uint16_t RcvBufferSize;

    int Errno;

};

#endif
