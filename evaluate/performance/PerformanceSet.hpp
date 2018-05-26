#ifndef PerformanceSet_hpp
#define PerformanceSet_hpp

#include "Thread.hpp"
#include "Helper.hpp"
#include "ConfigFile.hpp"

#include <sys/types.h>

#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <vector>
#include <memory>
#include <fstream>
#include <ctime>
#include <chrono>
#include <iomanip>

using namespace std;

class Perf
{

public:

    Perf(const string);
    ~Perf();

    void generatePayload();
    void spawnThreads();
    void waitThreads();

    Thread::RunThreadsP_t ThreadsRun;

private:

    string ConfigFilename;
    uint ThreadCount;

    vector<unique_ptr<Thread>> ThreadObjects;
    vector<unique_ptr<Thread>>::iterator ThreadObjectsIter;

};

#endif
