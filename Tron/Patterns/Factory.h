#ifndef FACTORY_H
#define FACTORY_H


#include "../Core/Object.h"
#include "../Gui/Bar.h"
#include "../Gui/Menu.h"
#include "../Game/Player.h"
#include "../Core/Map.h"


class Factory
{
public:
    Player* Create( SDL_Renderer *rnd, CMap *map, Circle snakePos, Point snakeSpeed, Point snakeSpeedBorder, 
                    SDL_Color snakeColor, const Rect &barPos, const string &barPath);
    Menu* Create(TTF_Font *textFont);
};


#endif