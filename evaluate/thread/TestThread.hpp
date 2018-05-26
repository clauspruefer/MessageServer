#include <sys/un.h>
#include <string>
#include <thread>
#include <atomic>
#include <queue>

using namespace std;

class TestDummy
{

public:

    TestDummy();
    ~TestDummy();

    void setValue(unsigned int);
    unsigned int getValue();

private:
    unsigned int TestValue;

};

class TestThread
{

public:

    TestThread();
    ~TestThread();

    static void processThreadA(TestDummy*);
    static void processThreadB(TestDummy*);

    static void processQueue(volatile std::atomic<bool>&, std::queue<int>&);

private:

};
