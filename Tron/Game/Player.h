#ifndef _GAME_PLAYER_H
#define _GAME_PLAYER_H


#include "../Gui/Bar.h"
#include "Snake.h"


class Player
{
public:
    Player();
    ~Player();
    void SetBar(Bar *bar);
    void SetSnake(Snake *snk);
    Snake* GetSnake();
    void SyncWithSnake();

    void ScoresUp();
    Uint32 GetScores();
private:
    Bar *_energy; // delete at destructor
    Snake *_snake; // delete at destructor
    Uint32 _scores;
};


#endif
