#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "Gui/Menu.h"
#include "Gui/Bar.h"

#include "Core/Event.h"
#include "Core/Converter.h"
#include "Core/Map.h"

#include "Events/Event_MainMenu.h"
#include "Events/Event_Game.h"

#include "Core/ConfigFile.h"
#include "Game/MusicController.h"

#include <iostream>
#include <vector>
using namespace std;


#define MAIN_ERROR(x) \
{ \
    cout << x << endl; \
    cout << SDL_GetError(); \
    return 1; \
}


int main(int argc, char *argv[])
{
    srand((int)time(0));

    if( SDL_Init(SDL_INIT_EVERYTHING) < 0)
        MAIN_ERROR("Can't init SDL");
    if( TTF_Init() < 0)
        MAIN_ERROR("Can't init TTF");
    if( Mix_Init(MIX_INIT_OGG) == 0)
        MAIN_ERROR("Can't init Mix");

    ConfigFile &cf = Singleton<ConfigFile>::GetInstance();
    Uint32 wnd_w = 800, wnd_h = 600;
    Uint32 flags = 0;
    try {
        wnd_w = cf.GetField_Uint32("window_w");
        wnd_h = cf.GetField_Uint32("window_h");
        if(cf.GetField_bool("fullscreen"))
            flags |= SDL_WINDOW_FULLSCREEN;
        if(cf.GetField_bool("borderless"))
            flags |= SDL_WINDOW_BORDERLESS;
    }
    catch(Exception e)
    {
        cout << e.What() << endl;
    }

    SDL_Window *wnd = SDL_CreateWindow("Tron", 25, 25, wnd_w, wnd_h, flags);
    if(wnd == nullptr)
        MAIN_ERROR("Can't create window");
    SDL_Renderer *rnd = SDL_CreateRenderer(wnd, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_TARGETTEXTURE);
    if(rnd == nullptr)
        MAIN_ERROR("Can't create renderer");
        
    SDL_Texture *texture = SDL_CreateTexture(rnd, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, wnd_w, wnd_h);
    if(texture == nullptr)
        MAIN_ERROR("Can't create target texture");
    
    Singleton<Gui>::GetInstance().Init(rnd);
    Converter::InitGui(wnd_w, wnd_h);

    Event *myev = nullptr;
    MusicController &mc = MusicController::GetInstance();
    try
    {
        mc.Init();
    }
    catch(Exception e)
    {
        MAIN_ERROR(e.What());
    }

    Uint32 time_end = SDL_GetTicks() + 1000, curr_ticks = 0;
    while(1)
    {
        ++curr_ticks;
        if(SDL_GetTicks() >= time_end)
        {
            //cout << curr_ticks << endl;
            curr_ticks = 0;
            time_end = SDL_GetTicks() + 1000;
        }
        
        mc.Action();
        SDL_SetRenderTarget(rnd, texture);
        try
        {
            if(myev == nullptr)
            {
                Factory factory;
                myev = new Event_Game(wnd, rnd, &factory);
            }
            myev->GetEvents();
        }
        catch(Event *newev)
        {
            delete myev;
            myev = newev;
        }
        catch(Exception e)
        {
            delete myev;
            myev = nullptr;
            MAIN_ERROR(e.What());
        }
        if(myev == nullptr)
            break;
        
        SDL_SetRenderTarget(rnd, nullptr);
        SDL_RenderCopy(rnd, texture, nullptr, nullptr);
        SDL_RenderPresent(rnd);
    }

    mc.Quit();
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();

    return 0;
}
