#include "MessageServer.hpp"

using namespace std;

int main()
{

    cout << "### >> ---------------------------------------------------------------" << endl;
    cout << "### >> WCDX Message Server " <<  MSG_SERVER_VERSION << endl;
    cout << "### >> ---------------------------------------------------------------" << endl;

#ifdef UNITTEST_MSGS
    cout << "### >> - Unittest version running" << endl;
#endif

    string UnixDomainSocket = SERVER_SOCKET;

    MsgServer MsgServerObj;

    MsgServerObj.setUnixDomainSocket(UnixDomainSocket);
    MsgServerObj.Init();
    MsgServerObj.dropPrivileges();
    MsgServerObj.ServerLoop();

    return 0;

}
