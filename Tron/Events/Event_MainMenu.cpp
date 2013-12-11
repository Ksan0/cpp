#include "Event_MainMenu.h"
#include <iostream>
using namespace std;

Event_MainMenu::Event_MainMenu(SDL_Window *wnd, SDL_Renderer *rnd, Factory *factory) : Event(wnd, rnd)
{
    Uint32 fontsize;
    Converter::ScreenPercentToPixel(0, 0.03f, nullptr, &fontsize);

    menufont = TTF_OpenFont("ttf/Chuck Noon.ttf", fontsize);
    if(menufont == nullptr)
        throw Exception("Can't load font");
    menu = factory->Create(menufont);
}

Event_MainMenu::~Event_MainMenu()
{
    if(menu != nullptr)
        delete menu;
    if(menufont != nullptr)
        TTF_CloseFont(menufont);
}

void Event_MainMenu::KeyDown(SDL_Keycode key)
{
    if(key == SDLK_RETURN)
    {
        string str = menu->GetFocusedString();
        if(str == "Play")
            throw string("play");
        if(str == "Restart")
            throw string("restart");
        if(str == "Quit")
            throw string("exit");
    } else {
        menu->KeyDown(key);
    }

    if(key == SDLK_ESCAPE)
        throw string("play");
}

void Event_MainMenu::KeyUp(SDL_Keycode key)
{
}

void Event_MainMenu::Action(float sPast)
{
}
