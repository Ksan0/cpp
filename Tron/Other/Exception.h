#ifndef _EXCEPTION_H
#define _EXCEPTION_H


class Exception
{
public:
    Exception(const char *msg);
    Exception(const Exception &);
    const char* What() const;
private:
    Exception& operator=(const Exception &);

    const char *const _what; // don't delete at destructor
};


#endif
