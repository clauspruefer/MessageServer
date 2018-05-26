#include "Main.hpp"

using namespace std;

Main::Main(server_rec* Rec):
    ServerRec(Rec)
{
    ap_log_error("msg", 1, APLOG_MODULE_INDEX, APLOG_ERR, 0, ServerRec, "Starting MsgServer Apache Module Instance");
    ap_log_error("msg", 1, APLOG_MODULE_INDEX, APLOG_ERR, 0, ServerRec, "Setup Connection to Unix Domain Socket");
    ap_log_error("msg", 1, APLOG_MODULE_INDEX, APLOG_ERR, 0, ServerRec, SERVER_SOCKET);

    ConnSetObj.setUnixDomainSocketName(SERVER_SOCKET);
    ConnSetObj.connect();

}

Main::~Main()
{
    ConnSetObj.close();
}

int Main::run(request_rec* Req)
{

    //- accept GET request
    if (!strcmp(Req->method, "GET")) {

        //- new request object
        unique_ptr<Request> RequestObj(new Request(this, Req));

        //- process request
        const int ReturnCode = RequestObj->process();

        return ReturnCode;
    }
    else {
        ap_log_error("msg", 1, APLOG_MODULE_INDEX, APLOG_ERR, 0, Req->server, "Method %s not allowed", Req->method);
        return DECLINED;
    }

}
