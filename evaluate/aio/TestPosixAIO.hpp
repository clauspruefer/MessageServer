#include <sys/un.h>
#include <aio.h>
#include <signal.h>

using namespace std;

class TestAIO
{

 public:

    TestAIO();
    ~TestAIO();

    static void processThreadA();
    static void processThreadB();

    static void callbackAsyncIOCompletion(int, siginfo_t*, void*);

 private:

};
