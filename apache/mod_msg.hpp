#ifndef mod_msg_hpp
#define mod_msg_hpp

#ifdef __cplusplus
#define EXTERN_C_BLOCK_BEGIN    extern "C" {
#define EXTERN_C_BLOCK_END      }
#define EXTERN_C_FUNC           extern "C"
#else
#define EXTERN_C_BLOCK_BEGIN
#define EXTERN_C_BLOCK_END
#define EXTERN_C_FUNC
#endif

#include "httpd.h"
#include "http_protocol.h"
#include "http_config.h"
#include "http_log.h"

#include "util_script.h"

EXTERN_C_BLOCK_BEGIN
APLOG_USE_MODULE(msg);
EXTERN_C_BLOCK_END

#endif
