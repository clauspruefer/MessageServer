#ifndef ConfigFile_hpp
#define ConfigFile_hpp

#include "../../Debug.cpp"

#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>

using namespace std;

class ConfigFile {

public:

    ConfigFile(const string);
    ~ConfigFile();

    void open();
    void parse();
    string getParamValue(const string);

private:

    string Filename;
    ifstream ifs;

    unordered_map<string, string> ConfigData;
    unordered_map<string, string>::iterator ConfigDataIter;

};

#endif
