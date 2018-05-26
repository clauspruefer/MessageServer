#include "ConnectionLocal.hpp"

using namespace std;

ConnectionLocal::ConnectionLocal()
{
    DBG(95, "Constructor");
}

ConnectionLocal::ConnectionLocal(const ConnectionLocal& ConnObj)
{
    DBG(95, "Copy Constructor");

    ConnectionType = ConnObj.ConnectionType;
    DomainSocket = ConnObj.DomainSocket;
    SocketStruct = ConnObj.SocketStruct;
    ConnStatus = ConnObj.ConnStatus;
    SocketFD = ConnObj.SocketFD;
}

ConnectionLocal::~ConnectionLocal()
{
    DBG(95, "Destructor");
}

void ConnectionLocal::setUnixDomainSocketName(string Name)
{
    DomainSocket = Name;
}

void ConnectionLocal::connect()
{

    DBG(10, "ConnectionLocal connect() to DomainSocket:" << DomainSocket);

    SocketFD = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    SocketStruct.sun_family = AF_UNIX;
    strcpy(SocketStruct.sun_path, DomainSocket.c_str());

    ERR("ConnectionLocal connect() to DomainSocket:" << DomainSocket);

    int rc = ::connect(
        SocketFD,
        reinterpret_cast<struct sockaddr*>(&SocketStruct),
        sizeof(SocketStruct)
    );

    if (rc != 0) {
        ERR("connect() - error rc:" << rc << " errno:" << errno << " strerror:" << strerror(errno));
        ConnStatus = false;
    }

}

void ConnectionLocal::reconnect()
{
    this->connect();
    this_thread::sleep_for(chrono::seconds(CLIENTLIB_TRANSMIT_RETRY_SLEEP_INTERVAL_SEC));
}

void ConnectionLocal::sendMessage(const stringstream& Message)
{
    setSendStatus(::write(SocketFD, Message.str().c_str(), Message.str().length()));
}

void ConnectionLocal::sendMessage(const char* Message, const int& MessageLength)
{
    setSendStatus(::write(SocketFD, Message, MessageLength));
}

void ConnectionLocal::setSendStatus(int rc)
{
    if (rc == 0) {
        ERR("Endpoint closed connection");
        ConnStatus = false;
        return;
    }

    if (rc == -1) {
        ERR("Error errno:" << errno << " strerror:" << strerror(errno));
        SendStatus = false;
        Errno = errno;
        return;
    }

    ConnStatus = true;
    SendStatus = true;

}

void ConnectionLocal::sendMessageReconnect(const stringstream& Message)
{

    int ReconnectCount = 0;

    while(ReconnectCount < CLIENTLIB_TRANSMIT_RETRY_COUNT) {

        sendMessage(Message);

        if (ConnStatus == true && SendStatus == true) { return; }

        if (ConnStatus == false) {
            ERR("Connection Status broken. Try reconnect.");
            reconnect();
        }
    }

}

void ConnectionLocal::receiveMessage(string& Message)
{

    //char RecvBuffer[RcvBufferSize];
    int rc = ::read(SocketFD, RecvBuffer, MSGBUFFER_SIZE);

    if (rc == 0) {
        ERR("Endpoint closed connection");
        ConnStatus = false;
        return;
    }

    if (rc == -1) {
        ERR("Error errno:" << errno << " strerror:" << strerror(errno));
        RecvStatus = false;
        Errno = errno;
        return;
    }

    RecvBytes = rc;

    Message.assign(RecvBuffer, RecvBytes);

    ConnStatus = true;
    RecvStatus = true;

}
