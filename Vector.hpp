#ifndef Vector_hpp
#define Vector_hpp

#include <vector>
#include <iterator>

#include "Debug.cpp"


using namespace std;


//- overloaded vector class providing multi element erase functionality
template <class T>
class Vector: public vector<T> {

public:

    using vector<T>::erase;

    Vector() {
        DBG(120, "Contructor");
    }

    ~Vector() {
        DBG(120, "Destructor");
    }

    void erase(const vector<uint>& EraseElements)
    {
        for(uint i=0; i<EraseElements.size(); ++i) {
            DBG(120, "Erase Element at index:" << i);
            erase(this->begin()+EraseElements.at(i));
        }
    }


private:

protected:


};

#endif
