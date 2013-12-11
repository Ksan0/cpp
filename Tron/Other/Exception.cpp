#include "Exception.h"


Exception::Exception(const char *msg) : _what(msg)
{
}

Exception::Exception(const Exception &ex) : _what(ex._what)
{
}

const char* Exception::What() const
{
    return _what != nullptr ? _what : "NULL EXCEPTION";
}