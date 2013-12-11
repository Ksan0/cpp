#ifndef _CONFIG_FILE_H
#define _CONFIG_FILE_H


#include "../Patterns/Singleton.h"
#include <string.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
using std::vector;
using std::string;
using std::stringstream;
using std::ifstream;
using std::ios;
using std::getline;



class ConfigFile
{
public:
    string GetField(const string &name);
    bool GetField_bool(const string &name);
    unsigned int GetField_Uint32(const string &name);
private:
    ConfigFile();
    ConfigFile(const ConfigFile &);
    ConfigFile& operator=(const ConfigFile &);

    friend class Singleton<ConfigFile>;
};


template<>
class Singleton<ConfigFile>
{
public:
    static ConfigFile& GetInstance();
};


#endif
