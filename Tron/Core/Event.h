#ifndef _CORE_EVENT_LIST_H
#define _CORE_EVENT_LIST_H


#include <SDL.h>
#include "Object.h"


class Event
{
public:
    Event(SDL_Window *wnd, SDL_Renderer *rnd);
    virtual ~Event();
    void GetEvents();
protected:
    SDL_Renderer *renderer; // don't delete at destructor
    SDL_Window *window; // don't delete at destructor
    void SetLastCallNow();
private:
    virtual void KeyDown(SDL_Keycode key);
    virtual void KeyUp(SDL_Keycode key);
    virtual void Action(float sPast) = 0;
    Uint32 _lastCallInTime;
};


#endif
