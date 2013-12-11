#ifndef _SINGLTON_H
#define _SINGLTON_H


#include "../Other/Exception.h"


template <typename T>
class Singleton
{
public:
    static T& GetInstance()
    {
        throw Exception("Class isn't singleton");
    }
};


#endif
