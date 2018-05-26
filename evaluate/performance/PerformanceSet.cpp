#include "PerformanceSet.hpp"

#include "../../Debug.cpp"
#include "../../Constant.hpp"


using namespace std;


//- "perf" class
Perf::Perf(const string ConfigFile) :
    ThreadsRun(new Thread::RunThreads_t(false)),
    ConfigFilename(ConfigFile)
{
    DBG(180, "Constructor");
}

Perf::~Perf()
{
    DBG(180, "Destructor");
}

void Perf::generatePayload()
{

    //- read config file
    ConfigFile ConfigFileObj(ConfigFilename);
    ThreadCount = stoi(ConfigFileObj.getParamValue("ThreadCount"));

    //- init session id
    SessionIDVec_t SessionVector(SESSION_ID_BYTES/2);
    SessionVector = { 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    Session S;
    S.initID(SessionVector);

    //- generate thread objects
    for(uint i=0; i<ThreadCount; ++i) {

        uint SessionAmount = stoi(ConfigFileObj.getParamValue("SessionAmount"));
        uint MessageAmount = stoi(ConfigFileObj.getParamValue("MessageAmount"));

        unique_ptr<Thread> ThreadObj(new Thread(i, ThreadsRun, SessionAmount, MessageAmount));

        ThreadObj->setMessageType(ConfigFileObj.getParamValue("MessageType"));
        ThreadObj->setConnectionType(ConfigFileObj.getParamValue("ConnectionType"));
        ThreadObj->setMessageLengthMin(stoi(ConfigFileObj.getParamValue("MessageLengthMin")));
        ThreadObj->setMessageLengthMax(stoi(ConfigFileObj.getParamValue("MessageLengthMax")));

        ThreadObj->generateMessages(&S);

        ThreadObjects.push_back(move(ThreadObj));

    }

}

void Perf::spawnThreads()
{
    for(uint i=0; i<ThreadCount; ++i) {
        ThreadObjects.at(i)->spawn();
    }
}

void Perf::waitThreads()
{
    for(uint i=0; i<ThreadCount; ++i) {
        ThreadObjects.at(i)->waitFinished();
    }
}

//- main
int main(int argc, const char* argv[])
{

    //- init random
    srand (time(NULL));

    //- get process id
    uint ProcessID = getpid();

    if (argc != 2) {
        cerr << "Invalid command line parameter count" << endl;
        exit(1);
    }

    const string ConfigFilename(argv[1], strlen(argv[1]));

    //- initialize and process thread objects
    try {

        Perf PerfObj(ConfigFilename);
        PerfObj.generatePayload();

        //- spawn threads
        PerfObj.spawnThreads();

        //- notify python unittest, that perf can be attached

        string ProcessDir("/tmp");
        string FileMsgServerPrefix("wcdxmsgserver");
        string FilePerfPrefix("perftest");

        stringstream FilePrefix;
        FilePrefix << ProcessDir << "/" << FileMsgServerPrefix << "." << FilePerfPrefix << ".";

        stringstream FileName;
        FileName << FilePrefix.str() << "threads.ready";

        ofstream FileThreadsReady;
        FileThreadsReady.open(FileName.str());
        FileThreadsReady << "ready";
        FileThreadsReady.close();

        DBG(80, "Wait for python attached perf");

        //- wait for python unittest attached perf
        FileName.clear();
        FileName << FilePrefix.str() << "threads.runnow";

        while(File::FileExists(FileName) == false) {
            this_thread::sleep_for(chrono::milliseconds(1));
        }

        this_thread::sleep_for(chrono::seconds(2));

        DBG(80, "Start running threads");

        //- start threads processing (sending messages)
        PerfObj.ThreadsRun->store(true);

        clock_t c_start = clock();
        auto t_start = chrono::high_resolution_clock::now();

        DBG(80, "Wait joining threads");

        //- wait until threads joined
        PerfObj.waitThreads();

        DBG(80, "Joined threads");

        clock_t c_end = clock();
        auto t_end = chrono::high_resolution_clock::now();

        cout << fixed << setprecision(2) << "CPUTime:" << 1000.0*(c_end-c_start)/CLOCKS_PER_SEC << "ms" << endl;
        cout << " ProcessID:" << ProcessID << " WallClockTime:" << chrono::duration<double, milli>(t_end-t_start).count() << "ms" << endl;

    }
    catch( const char* msg ) {
        cerr << "Thread Processing failure:" << msg << endl;
        exit(1);
    }

    DBG(-1, "Performance test completed");

    return 0;

}
