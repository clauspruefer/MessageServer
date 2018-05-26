#include "Connection.hpp"

using namespace std;

Connection::Connection():
    ConnectionType("persistent"),
    ConnStatus(true)
{
    DBG(95, "Constructor");
}

Connection::~Connection()
{
    DBG(95, "Destructor");
}

void Connection::setType(string Type)
{
    ConnectionType = Type;
}

void Connection::close()
{
    int rc = ::close(SocketFD);
    if (rc != 0) {
        ERR("Errror closing socket. rc:" << rc << " errno:" << errno << " strerror:" << strerror(errno));
    }

}
