#include "Event.h"


Event::Event(SDL_Window *wnd, SDL_Renderer *rnd)
{
    SetLastCallNow();
    renderer = rnd;
    window = wnd;
}
Event::~Event()
{
}
void Event::GetEvents()
{
    SDL_Event ev;
    while(SDL_PollEvent(&ev))
    {
        if(ev.type == SDL_KEYDOWN)
        {
            KeyDown(ev.key.keysym.sym);
        } else if(ev.type == SDL_KEYUP)
        {
            KeyUp(ev.key.keysym.sym);
        } else if(ev.type == SDL_QUIT)
        {
            throw (Event*)nullptr;
        }
    }

    float sPast = (SDL_GetTicks() - _lastCallInTime) / 1000.f;
    _lastCallInTime = SDL_GetTicks();
    Action(sPast);
    Singleton<Gui>::GetInstance().Action();
}
void Event::SetLastCallNow()
{
    _lastCallInTime = SDL_GetTicks();
}

void Event::KeyDown(SDL_Keycode key)
{
}
void Event::KeyUp(SDL_Keycode key)
{
}
void Event::Action(float sPast)
{
}