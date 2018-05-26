#include <sys/timerfd.h>
#include <time.h>
#include <stdio.h>
#include <aio.h>
#include <signal.h>
#include <strings.h>

#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#include <iostream>
#include <thread>
#include <chrono>

#include "TestPosixAIO.hpp"

using namespace std;

TestAIO::TestAIO()
{
    cout << "### TestPosixAIO Constructor" << endl;
}

TestAIO::~TestAIO()
{
    cout << "### TestPosixAIO Destructor" << endl;
}

void TestAIO::processThreadA() {
    cout << "Process Thread A." << endl;
}

void TestAIO::processThreadB() {

    struct itimerspec TimeoutTime;

    int TimerFd = timerfd_create(CLOCK_MONOTONIC, 0);
    cout << "Timeout FileDescriptorID:" << TimerFd << endl;

    TimeoutTime.it_value.tv_sec = 5;
    TimeoutTime.it_value.tv_nsec = 0;

    timerfd_settime(TimerFd, 0, &TimeoutTime, nullptr);

    struct sigaction AIOSignal;
    struct aiocb AIOCallback;

    sigemptyset(&AIOSignal.sa_mask);

    AIOSignal.sa_flags = SA_SIGINFO;
    AIOSignal.sa_sigaction = TestAIO::callbackAsyncIOCompletion;

    bzero((char*)&AIOCallback, sizeof(struct aiocb));

    AIOCallback.aio_fildes = TimerFd;
    AIOCallback.aio_buf = malloc(9);
    AIOCallback.aio_nbytes = 8; //- 8 byte integer return value

    AIOCallback.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
    AIOCallback.aio_sigevent.sigev_signo = SIGIO;
    AIOCallback.aio_sigevent.sigev_value.sival_ptr = &AIOCallback;

    int rc = sigaction(SIGIO, &AIOSignal, nullptr);
    cout << "sigaction() rc:" << rc << endl;

    rc = aio_read(&AIOCallback);
    cout << "aio_read() rc:" << rc << endl;

    this_thread::sleep_for(chrono::seconds(2));
    cout << "Process slept 2sec." << endl;

    this_thread::sleep_for(chrono::seconds(2));
    cout << "Process slept 2sec." << endl;

    this_thread::sleep_for(chrono::seconds(2));
    cout << "Process slept 2sec." << endl;

    this_thread::sleep_for(chrono::seconds(2));
    cout << "Process slept 2sec." << endl;

    this_thread::sleep_for(chrono::seconds(2));
    cout << "Process slept 2sec." << endl;

    this_thread::sleep_for(chrono::seconds(2));
    cout << "Process slept 2sec." << endl;

}

void TestAIO::callbackAsyncIOCompletion(int signo, siginfo_t *info, void* context) {

    struct aiocb *req;
    req = (struct aiocb*)info->si_value.sival_ptr;
    cout << "### TIMEOUT SIGNAL CALLBACK ok" << endl;

    if (aio_error(req) == 0) {

        int ResultLength = aio_return(req);
        cout << "### TIMEOUT SIGNAL CALLBACK ResultLength:" << ResultLength << endl;

    }

}

int main()
{

    cout << "Testing POSIX AIO:" << endl;

    thread a (&TestAIO::processThreadA);
    thread b (&TestAIO::processThreadB);

    a.join();
    b.join();

    return 0;

}
