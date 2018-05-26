#ifndef Global_hpp
#define Global_hpp

#include <atomic>
#include <queue>
#include <memory>

using namespace std;

//- message transfer struct
struct MessageTransfer_t {
    uint16_t ClientFD;        //- client filedescriptor
    char* MsgBufferPointer;   //- message buffer pointer
    uint16_t MsgBytes;        //- message length
};

//- global typedefs
typedef volatile atomic<int> QueueLock_t;
typedef queue<MessageTransfer_t> Queue_t;

typedef shared_ptr<QueueLock_t> QueueLockPointer_t;
typedef shared_ptr<Queue_t> QueuePointer_t;

#endif
