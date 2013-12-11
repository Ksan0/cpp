#include "Player.h"


Player::Player() 
{
    _scores = 0;
    _energy = nullptr;
}
Player::~Player()
{
    if(_energy != nullptr)
        delete _energy;
    if(_snake != nullptr)
        delete _snake;
}
void Player::SetBar(Bar *bar)
{
    _energy = bar;
}
void Player::SetSnake(Snake *snk)
{
    _snake = snk;
}
Snake* Player::GetSnake()
{
    return _snake;
}
void Player::SyncWithSnake()
{
    _energy->SetVal(_snake->GetEnergy());
}


void Player::ScoresUp()
{
    ++_scores;
}
Uint32 Player::GetScores()
{
    return _scores;
}
