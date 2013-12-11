#ifndef _EVENT_LIST_MAIN_MENU_H
#define _EVENT_LIST_MAIN_MENU_H


#include "../Core/Event.h"
#include "Event_All.h"
#include "../Gui/Menu.h"
#include <SDL_ttf.h>
#include "../Patterns/Factory.h"


class Event_MainMenu : public Event
{
public:
    Event_MainMenu(SDL_Window *wnd, SDL_Renderer *rnd, Factory *factory);
    ~Event_MainMenu();
    void KeyDown(SDL_Keycode key);
    void KeyUp(SDL_Keycode key);
    void Action(float sPast);
private:
    TTF_Font *menufont; // delete at destructor
    Menu *menu; // delete at destructor
};

#endif
