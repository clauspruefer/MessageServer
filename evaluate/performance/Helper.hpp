#ifndef Helper_hpp
#define Helper_hpp

#include "../../Debug.cpp"

#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>

using namespace std;

class Random {

public:

    static int generateRandIntRange(const uint To, const uint From)
    {
        return rand() % From + To;
    }

    static uint8_t generateRandUint8()
    {
        return (uint8_t)Random::generateRandIntRange(0, 255);
    }

    static uint8_t generateRandUint8Ascii()
    {
        return (uint8_t)Random::generateRandIntRange(32, 64);
    }

};

class Generator {

public:

    template <class FWIterator, class Generator>
    static void generate(FWIterator Element, FWIterator LastElement, Generator genFunction)
    {

        //- loop on elements until last iterator element reached
        while (Element != LastElement) {

            //- generate element by generator function
            *Element = genFunction();

            //- increment element iterator pointer
            ++Element;

        }

    }

};

class File {

public:

    static bool FileExists(const stringstream& Filename)
    {
        ifstream File(Filename.str());
        return true;
    }

};

#endif
