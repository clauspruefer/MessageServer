#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <iostream>
#include <thread>
#include <chrono>

#include "TestThread.hpp"

using namespace std;

TestDummy::TestDummy()
{
    cout << "Test Dummy Constructor." << endl;
    TestValue = 5;
}

TestDummy::~TestDummy()
{
    cout << "Test Dummy Destructor." << endl;
}

void TestDummy::setValue(unsigned int value) {
    TestValue = value;
}

unsigned int TestDummy::getValue() {
    return TestValue;
}

TestThread::TestThread()
{
    cout << "TestThread Constructor." << endl;
}

TestThread::~TestThread()
{
    cout << "TestThread Destructor." << endl;
}

void TestThread::processThreadA(TestDummy* TestObj) {
    cout << "Process Thread A." << endl;
    cout << "TestValue:" << TestObj->getValue() << endl;
}

void TestThread::processThreadB(TestDummy* TestObj) {
    cout << "Process Thread B." << endl;
    this_thread::sleep_for(chrono::seconds(4));
    TestObj->setValue(7);
}

void TestThread::processQueue(volatile std::atomic<bool> &QueueLock, std::queue<int> &Queue) {

    int i=0;
    while(i<10000) {
        if (QueueLock == false) {
            QueueLock = true;
            if (Queue.size() > 0) {
                cout << Queue.front() << endl;
                Queue.pop();
            }
            QueueLock = false;
        }
        this_thread::sleep_for(chrono::milliseconds(1));
        ++i;
    }
}

int main()
{

    cout << "Testing Threads Object Reference passing (non atomic)." << endl;

    TestDummy* TestObj = new TestDummy();

    thread a (&TestThread::processThreadA, std::ref(TestObj));
    thread b (&TestThread::processThreadB, std::ref(TestObj));

    a.join();
    b.join();

    cout << "TestValue:" << TestObj->getValue() << endl;

    cout << "Testing Thread Synchronization with atomic values." << endl;

    std::queue<int> Queue;
    volatile std::atomic<bool> QueueLock;

    std::atomic_init(&QueueLock, true);

    thread c (&TestThread::processQueue, std::ref(QueueLock), std::ref(Queue));

    this_thread::sleep_for(chrono::seconds(2));

    Queue.push(100);
    Queue.push(101);
    Queue.push(102);

    QueueLock = false;

    this_thread::sleep_for(chrono::seconds(1));

    while (QueueLock == true) { };

    QueueLock = true;
    Queue.push(103);
    QueueLock = false;

    this_thread::sleep_for(chrono::seconds(5));

    c.join();

    return 0;

}
