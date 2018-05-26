#include "mod_msg.hpp"
#include "Main.hpp"

EXTERN_C_BLOCK_BEGIN
typedef struct
{
    void* MainObjRef;
}
MainObjRef_t;
EXTERN_C_BLOCK_END

EXTERN_C_FUNC
int msg_handler(request_rec* Req)
{

    if (!Req->handler || strcmp(Req->handler, "msg")) { return (DECLINED); }

    //- set result mime type
    ap_set_content_type(Req, "application/json");

    //- get main c++ object pointer
    MainObjRef_t* ObjRef;

    apr_pool_userdata_get(
        reinterpret_cast<void**>(&ObjRef),
        "MSGSERVER_MAIN_CPLUSPLUS_OBJECT",
        Req->server->process->pool
    );

    //- run
    Main* MainObj;
    MainObj = static_cast<Main*>(ObjRef->MainObjRef);

    return MainObj->run(Req);

}

EXTERN_C_FUNC
void msg_child_init(apr_pool_t* pool, server_rec* server)
{

    //- setup main c++ object
    Main* MainObj = new Main(server);

    //- store main object reference object pointer in server pool
    MainObjRef_t* ObjRef = static_cast<MainObjRef_t*>(apr_palloc(pool, sizeof(MainObjRef_t)));
    ObjRef->MainObjRef = static_cast<void*>(MainObj);

    //- store user data in process pool
    apr_pool_userdata_set(
        static_cast<const void*>(ObjRef),
        "MSGSERVER_MAIN_CPLUSPLUS_OBJECT",
        NULL,
        server->process->pool
    );

}

EXTERN_C_FUNC
void msg_hooks(apr_pool_t* pool)
{
    //- set request handler
    ap_hook_handler(msg_handler, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_child_init(msg_child_init, NULL, NULL, APR_HOOK_MIDDLE);
}

EXTERN_C_BLOCK_BEGIN
module AP_MODULE_DECLARE_DATA msg_module =
{
    STANDARD20_MODULE_STUFF,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    msg_hooks
};
EXTERN_C_BLOCK_END
