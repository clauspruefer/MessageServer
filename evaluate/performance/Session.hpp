#ifndef Session_hpp
#define Session_hpp

#include "Helper.hpp"

#include <string>
#include <vector>

typedef vector<uint8_t> SessionIDVec_t;
typedef vector<uint8_t>::iterator SessionIDVecIter_t;

using namespace std;

class Session
{

public:

    static string generateIDRand(uint Size)
    {
        SessionIDVec_t SessionIDVector(Size);
        Generator::generate(SessionIDVector.begin(), SessionIDVector.end(), Random::generateRandUint8);
        return SessionVector2String(SessionIDVector);
    }

    static string SessionVector2String(SessionIDVec_t SessionIDVector)
    {

        string SessionID = "";

        for (SessionIDVecIter_t it=SessionIDVector.begin(); it != SessionIDVector.end(); ++it) {

            int VectorValue = *it;
            stringstream ConvertHexStream;
            ConvertHexStream << hex << VectorValue;
            string HexOutput = ConvertHexStream.str();

            if (VectorValue >= 0 && VectorValue <=15) {
                SessionID.append("0");
            }

            SessionID.append(HexOutput);

        }

        return SessionID;
    }

    void initID(SessionIDVec_t SessionVector)
    {
        UpcountSessionID = SessionVector;
    }

    string generateID()
    {
        incrementID(UpcountSessionID.size()-1);
        return Session::SessionVector2String(UpcountSessionID);
    }

    void incrementID(uint ElementIndex)
    {

        int CheckValue = UpcountSessionID.at(ElementIndex);

        if (CheckValue == 255 && ElementIndex > 0) {
            UpcountSessionID[ElementIndex] = 0;
            incrementID(ElementIndex-1);
        }
        else {
            ++UpcountSessionID[ElementIndex];
        }

    }

private:

    SessionIDVec_t UpcountSessionID;

};

#endif
