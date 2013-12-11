#ifndef _EVENT_LIST_GAME_H
#define _EVENT_LIST_GAME_H


#include "../Patterns/Factory.h"
#include "../Core/Event.h"
#include "../Core/Map.h"
#include "Event_All.h"
#include "../Gui/Menu.h"
#include "../Game/Player.h"
#include "../Game/Impact.h"
#include "../Game/MusicController.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <sstream>
#include <iomanip>
using std::stringstream;
using std::string;
using std::noskipws;


class Event_Game : public Event
{
public:
    Event_Game(SDL_Window *wnd, SDL_Renderer *rnd, Factory *factory);
    ~Event_Game();
    void KeyDown(SDL_Keycode key);
    void KeyUp(SDL_Keycode key);
    void Action(float sPast);
private:
    void StartEventInside(Event *ev);
    void DrawScores();
    void ClearTop();
    bool PlayerAction(Player &pl, float maPast); // return true if current player lose
    void Restart();
    void TimerToGame(float sPast);

    CMap map;
    Player *one, *two; // delete at destructor
    
    TTF_Font *scores_font, *timer_font; // delete at destructor

    bool gameNeedRestart;
    float gameStartIn, gameRestartIn;
};


#endif
