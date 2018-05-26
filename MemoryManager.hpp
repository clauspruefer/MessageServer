#ifndef MemoryManager_hpp
#define MemoryManager_hpp

#include "Debug.cpp"

using namespace std;

template<class T>
class MemoryManager
{

public:

    MemoryManager(unsigned int SegmentCount, unsigned int SegmentSize):
        SegmentCount(SegmentCount),
        SegmentSize(SegmentSize)
    {
        DBG(120, "Contructor");
        SegmentOffset = 0;
        allocateMemory();
    }

    ~MemoryManager() {
        DBG(120, "Destructor");
        free(MemoryBaseAddress);
        DBG(120, "Freed memory");
    }

    T* getNextMemPointer() {
        SegmentOffset >= SegmentCount ? 0 : SegmentOffset++;
        return getMemPointer(SegmentOffset);
    }

    T* getMemBaseAddress() {
        return MemoryBaseAddress;
    }

private:

    unsigned int SegmentCount;
    unsigned int SegmentSize;
    unsigned int SegmentOffset;

    T* MemoryBaseAddress;

    void allocateMemory() {
        MemoryBaseAddress = static_cast<T*>(malloc(SegmentCount*sizeof(T)*SegmentSize));
        DBG(95, "Allocate Memory Address:" << static_cast<void*>(MemoryBaseAddress));
    }

    T* getMemPointer(unsigned int SegmentOffset) {

        T* MemPointer = MemoryBaseAddress;

        if (SegmentOffset < SegmentCount) {
            MemPointer += (SegmentOffset*SegmentSize)*sizeof(T);
        }

        DBG(95, "## Memory Base Address:" << static_cast<void*>(MemoryBaseAddress) << " SegmentOffset:" << SegmentOffset << " MemPointer Address:" << static_cast<void*>(MemPointer));

        return MemPointer;

    }

};

#endif
