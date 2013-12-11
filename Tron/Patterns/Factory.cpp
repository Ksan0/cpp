#include "Factory.h"


Menu* Factory::Create(TTF_Font *textFont)
{
    Menu *menu = new Menu;
    menu->SetTextColor(0, 0, 55, 155);
    menu->SetBgColor(155, 155, 255, 0);
    menu->SetPos(Point(0.40f, 0.25f));
    menu->SetFont(textFont);
    menu->AddItem(new MenuItem("Play"));
    menu->AddItem(new MenuItem("Restart"));
    menu->AddItem(new MenuItem("Quit"));
    return menu;
}


Player* Factory::Create(SDL_Renderer *rnd, CMap *map, Circle snakePos, Point snakeSpeed, Point snakeSpeedBorder, SDL_Color snakeColor, const Rect &barPos, const string &barPath)
{
    Player *player = new Player;
    
    Snake *snk = new Snake(snakePos, snakeSpeed, map);
    snk->SetSpeedBorder(snakeSpeedBorder.x, snakeSpeedBorder.y);
    snk->SetColor(snakeColor.r, snakeColor.g, snakeColor.b);
    player->SetSnake(snk);

    Bar *bar = new Bar(barPos);
    bar->SetMaxVal(100);
    bar->SetVal(50);
    bar->LoadTexture(rnd, barPath);
    player->SetBar(bar);

    return player;
}