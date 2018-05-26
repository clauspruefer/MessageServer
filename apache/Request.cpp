#include "Request.hpp"

using namespace std;

Request::Request(Main* MainObjRef, request_rec* Request):
    MainObj(MainObjRef),
    Req(Request),
    MessageType(""),
    MessageSession(""),
    MessagePayload(""),
    ReturnCode(DECLINED)
{
}

Request::~Request()
{
}

int Request::process()
{

    //- get URI parameter
    parseURI();

    //- process message
    processMessage();

    //- return processing result code
    return ReturnCode;

}

void Request::parseURI()
{
    apr_table_t *GET;
    ap_args_to_table(Req, &GET);

    const char* Type = apr_table_get(GET, "Type");
    const char* Session = apr_table_get(GET, "Session");
    const char* Payload = apr_table_get(GET, "Payload");

    if (Type) { MessageType.assign(Type); };
    if (Session) { MessageSession.assign(Session); };
    if (Payload) { MessagePayload.assign(Payload); };

    ap_log_error("msg", 1, APLOG_MODULE_INDEX, APLOG_DEBUG, 0, MainObj->ServerRec, "MessageType:%s", MessageType.c_str());
}

void Request::processMessage()
{

    //- return on incorrect message type
    if (MessageType != MSG_INDICATOR_GET && MessageType != MSG_INDICATOR_SET) {
        ap_log_error("msg", 1, APLOG_MODULE_INDEX, APLOG_DEBUG, 0, MainObj->ServerRec, "Message Indicator wrong Type:%s", MessageType.c_str());
        return;
    }

    //- return on incorrect session length
    if (MessageSession.length() != SESSION_ID_BYTES) {
        ap_log_error("msg", 1, APLOG_MODULE_INDEX, APLOG_DEBUG, 0, MainObj->ServerRec, "Wrong Session length.");
        return;
    }

    //- return on no payload (set)
    if (MessageType == MSG_INDICATOR_SET && MessagePayload.length() == 0) {
        ap_log_error("msg", 1, APLOG_MODULE_INDEX, APLOG_DEBUG, 0, MainObj->ServerRec, "No payload given.");
        return;
    }

    //- process set message type
    if (MessageType == MSG_INDICATOR_SET) {

        ap_log_error("msg", 1, APLOG_MODULE_INDEX, APLOG_DEBUG, 0, MainObj->ServerRec, "Msg received: SET");

        stringstream Message;
        Message << MSG_INDICATOR_SET << MessageSession << MessagePayload;

        //- send set message
        MainObj->ConnSetObj.sendMessageReconnect(Message);

        //- receive message container
        string RcvMessage;

        //- get result json
        MainObj->ConnSetObj.receiveMessage(RcvMessage);

        //- when server sends error, pass through
        if (MainObj->ConnSetObj.RecvStatus) {
            ap_rputs(RcvMessage.c_str(), Req);
        }

        ReturnCode = OK;

    }

    //- process get message type
    if (MessageType == MSG_INDICATOR_GET) {

        ap_log_error("msg", 1, APLOG_MODULE_INDEX, APLOG_DEBUG, 0, MainObj->ServerRec, "Msg received: GET");

        ConnectionLocal ConnGetObj = ConnectionLocal();
        ConnGetObj.setUnixDomainSocketName(SERVER_SOCKET);
        ConnGetObj.connect();

        stringstream Message;
        Message << MSG_INDICATOR_GET << MessageSession;

        //- send get message
        ConnGetObj.sendMessageReconnect(Message);

        //- receive message container
        string RcvMessage;

        //- get result json
        ConnGetObj.receiveMessage(RcvMessage);

        //- on send ok, put back server json
        if (ConnGetObj.RecvStatus) {
            ap_rputs(RcvMessage.c_str(), Req);
        }
        //- send error message
        else {
            stringstream ResultMsg;
            ResultMsg << "{ \"ResultCode\": " << E__MIDDLEWARE_RCV_ERROR << " }";
            ap_rputs(ResultMsg.str().c_str(), Req);
        }

        ConnGetObj.close();
        ReturnCode = OK;

    }

}
