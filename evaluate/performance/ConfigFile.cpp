#include "ConfigFile.hpp"

using namespace std;

ConfigFile::ConfigFile(const string File)
{
    DBG(80, "Constructor Filename:" << File);
    Filename = File;
    open();
    parse();
}

ConfigFile::~ConfigFile()
{
    DBG(80, "Destructor");
}

void ConfigFile::open()
{

    DBG(80, "Try opening file:" << Filename);
    ifs.open(Filename, ifstream::in);

    if (ifs.fail())
    {
        DBG(10, "Open file:" << Filename << "failed");
        exit(1);
    }

}

void ConfigFile::parse()
{

    DBG(80, "Try parsing file:" << Filename);
    try {

        string FileLine;

        while (getline(ifs, FileLine)) {

            string Delimiter = "=";

            string Key;
            string Value;

            Key = FileLine.substr(0, FileLine.find(Delimiter));
            Value = FileLine.substr(FileLine.find(Delimiter)+1, FileLine.back());

            ConfigData.insert(pair<string, string>(Key, Value));

        }
    }
    catch( const char* msg )
    {
        ERR("Parsing file:" << Filename << "failed with:" << msg);
        exit(1);
    }

}

string ConfigFile::getParamValue(const string Key)
{
    ConfigDataIter = ConfigData.find(Key);

    if (ConfigDataIter != ConfigData.end()) {
        return ConfigData.find(Key)->second;
    }

    return "notfound";
}
