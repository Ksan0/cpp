#include "ConfigFile.h"


ConfigFile::ConfigFile()
{
}


string ConfigFile::GetField(const string &name)
{
    ifstream file("config.txt");
    if(!file)
        throw Exception("No config file");

    auto split = [](char *buff, const char *delim, vector<string> &vec)
    {
        vec.clear();
        char *ptr = strtok(buff, delim);

        while(ptr != nullptr)
        {
            vec.push_back(ptr);
            ptr = strtok(nullptr, delim);
        }
    };

    char chbuff[1024];
    vector<string> vec;
    bool finded = false;

    while(!file.eof())
    {
        file.getline(chbuff, sizeof(chbuff));
        split(chbuff, " =", vec);
        if(vec.size() < 2 || vec[0] != name)
            continue;
        
        finded = true;
        break;
    }

    file.close();

    if(!finded)
        throw Exception("Not found param by name");

    return vec[1];
}


bool ConfigFile::GetField_bool(const string &name)
{
    string str = GetField(name);
    return str == "true";
}


unsigned int ConfigFile::GetField_Uint32(const string &name)
{
    unsigned int v;
    stringstream ss;
    ss << GetField(name);
    ss >> v;
    return v;
}


ConfigFile& Singleton<ConfigFile>::GetInstance()
{
    static ConfigFile cf;
    return cf;
}
