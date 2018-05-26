#ifndef ConnectionLocal_hpp
#define ConnectionLocal_hpp

#include "Connection.hpp"

using namespace std;

class ConnectionLocal: public Connection {

public:

    ConnectionLocal();
    ConnectionLocal(const ConnectionLocal&);
    ~ConnectionLocal();

    void setUnixDomainSocketName(const string);
    void connect();
    void reconnect();
    void sendMessage(const stringstream&);
    void sendMessage(const char*, const int&);
    void sendMessageReconnect(const stringstream&);
    void receiveMessage(string&);
    void setSendStatus(int);

private:

    string DomainSocket;

};

#endif
