#include "Event_Game.h"
#include <iostream>
using namespace std;


Event_Game::Event_Game(SDL_Window *wnd, SDL_Renderer *rnd, Factory *factory) : Event(wnd, rnd)
{
    Uint32 fontsize;
    Converter::ScreenPercentToPixel(0, 0.05f, nullptr, &fontsize);
    scores_font = TTF_OpenFont("ttf/Chuck Noon.ttf", fontsize);
    Converter::ScreenPercentToPixel(0, 0.05f, nullptr, &fontsize);
    timer_font = TTF_OpenFont("ttf/Chuck Noon.ttf", fontsize);

    if(scores_font == nullptr || timer_font == nullptr)
        throw Exception("Can't load font");

    Rect mapSize(0.02f, 0.10f, 0.96f, 0.88f);
    Uint32 dx, dy;
    Converter::ScreenPercentToPixel(mapSize.w, mapSize.h, &dx, &dy);
    Point worldSize((float)dx, (float)dy);
    map.LoadTexture(rnd, "img/field.bmp");
    map.SetScreenSize(mapSize);
    map.SetWorldSize(worldSize);
    Converter::InitMap(mapSize, worldSize);

    SDL_Color col_red = {255, 0, 0}, col_blue = {0, 0, 255};
    one = factory->Create(rnd, &map, Circle(0.25f*dx, 0.50f*dy, 5), Point( float(dx*0.2f), 0), Point(dx*0.1f, dx*0.4f), col_red, Rect(0.02f, 0.02f, 0.35f, 0.05f), "img/BarOne.bmp");
    two = factory->Create(rnd, &map, Circle(0.75f*dx, 0.50f*dy, 5), Point(-float(dx*0.2f), 0), Point(dx*0.1f, dx*0.4f), col_blue, Rect(0.98f, 0.02f, -0.35f, 0.05f), "img/BarTwo.bmp");

    map.Draw(rnd);
    DrawScores();
    Factory fac;
    StartEventInside(new Event_MainMenu(wnd, rnd, &fac));

    gameStartIn = 3;
    gameNeedRestart = false;
}

Event_Game::~Event_Game()
{
    if(one != nullptr)
        delete one;
    if(two != nullptr)
        delete two;
    if(scores_font != nullptr)
        TTF_CloseFont(scores_font);
    if(timer_font != nullptr)
        TTF_CloseFont(timer_font);
}

void Event_Game::StartEventInside(Event *ev)
{
    if(ev == nullptr)
        return;

    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);
    SDL_Texture *old_target = SDL_GetRenderTarget(renderer);
    SDL_Texture *new_target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetRenderTarget(renderer, new_target);
    SDL_RenderCopy(renderer, old_target, nullptr, nullptr);

    string outMessage;
    MusicController &mc = MusicController::GetInstance();

    while(1)
    {
        mc.Action();
        SDL_SetRenderTarget(renderer, new_target);
        try
        {
            ev->GetEvents();
        }
        catch(string s)
        {
            if (s == "exit")
                throw (Event*)nullptr;
            if (s == "play")
            {
                delete ev;
                break;
            }
            if (s == "restart")
            {
                delete ev;
                outMessage = s;
                break;
            }
        }
        SDL_SetRenderTarget(renderer, nullptr);
        SDL_RenderCopy(renderer, new_target, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    SDL_SetRenderTarget(renderer, old_target);
    SDL_DestroyTexture(new_target);

    if(outMessage == "restart")
        Restart();

    SetLastCallNow();
}


void Event_Game::DrawScores()
{
    string s;
    stringstream ss;
    ss << one->GetScores() << " | " << two->GetScores();
    std::getline(ss, s);
    SDL_Color col = {255, 255, 100, 255};
    SDL_Surface *text_surf = TTF_RenderText_Blended_Wrapped(scores_font, s.data(), col, -1);
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
    Uint32 x, y;
    Converter::ScreenPercentToPixel(0.5f, 0.025f, &x, &y);
    x -= text_surf->w/2;
    SDL_Rect rect = { x, y, text_surf->w, text_surf->h };
    SDL_RenderCopy(renderer, text_texture, nullptr, &rect);
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surf);
}


void Event_Game::ClearTop()
{
    Uint32 w, h;
    Converter::ScreenPercentToPixel(1, 0.10f, &w, &h);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect rect = {0, 0, w, h};
    SDL_RenderFillRect(renderer, &rect);
}


void Event_Game::KeyDown(SDL_Keycode key)
{   
    if(key == SDLK_ESCAPE)
    {
        Factory fac;
        StartEventInside(new Event_MainMenu(window, renderer, &fac));
    }

    if(key == SDLK_a || key == SDLK_d || key == SDLK_w || key == SDLK_s || key == SDLK_SPACE)
        one->GetSnake()->KeyDown(key);

    if(key == SDLK_UP || key == SDLK_DOWN || key == SDLK_LEFT || key == SDLK_RIGHT || key == SDLK_RCTRL)
        two->GetSnake()->KeyDown(key);
}

void Event_Game::KeyUp(SDL_Keycode key)
{
    if(key == SDLK_a || key == SDLK_d || key == SDLK_w || key == SDLK_s || key == SDLK_SPACE)
        one->GetSnake()->KeyUp(key);
    if(key == SDLK_UP || key == SDLK_DOWN || key == SDLK_LEFT || key == SDLK_RIGHT || key == SDLK_RCTRL)
        two->GetSnake()->KeyUp(key);
}

bool Event_Game::PlayerAction(Player &pl, float sPast)
{
    Snake *snake = pl.GetSnake();
    snake->MoveDrawFillMap(renderer, sPast);
    bool crash = false;
    if(Impact::impact(*snake, sPast) == Impact::Type::snake_crash)
    {
        if(!gameNeedRestart)
        {
            snake->SetActive(false);
            snake->CreateCrashEffect();
            crash = true;
        }
        //Uint32 x, y;
        //Converter::GameWorldToPixel(Impact::ImpactPoint.x, Impact::ImpactPoint.y, &x, &y);
        //filledCircleRGBA(renderer, x, y, 2, 0, 255, 0, 255);
    }
    pl.SyncWithSnake();
    return crash;
}


void Event_Game::Restart()
{
    map.Clear();
    Point worldSize = map.GetWorldSize();

    Snake *sn = one->GetSnake();
    sn->SetActive(true);
    sn->SetPos(Circle(0.25f*worldSize.x, 0.50f*worldSize.y, 5));
    sn->SetSpeed(Point(worldSize.x*0.2f, 0));
    sn->SetEnergy(50);
    sn->UpAllKeys();
    sn->SetSuperGhost(false);
    sn->RemoveCrashEffect();
    one->SyncWithSnake();

    sn = two->GetSnake();
    sn->SetActive(true);
    sn->SetPos(Circle(0.75f*worldSize.x, 0.50f*worldSize.y, 5));
    sn->SetSpeed(Point(-float(worldSize.x*0.2f), 0));
    sn->SetEnergy(50);
    sn->UpAllKeys();
    sn->SetSuperGhost(false);
    sn->RemoveCrashEffect();
    two->SyncWithSnake();

    map.Draw(renderer);

    gameStartIn = 3;
    gameNeedRestart = false;
}


void Event_Game::TimerToGame(float sPast)
{
    map.Draw(renderer);

    gameStartIn -= sPast;
    if(gameStartIn < 0)
        gameStartIn = 0;
    if(gameStartIn == 0)
        return;

    string s;
    stringstream ss;
    switch(Uint32(gameStartIn + 1))
    {
    case 4:
    case 3:
        ss << "ready";
        break;
    case 2:
        ss << "steady";
        break;
    case 1:
        ss << "go!";
        break;
    }
    //ss << Uint32(gameStartIn + 0.5f);
    std::getline(ss, s);

    SDL_Color col = {255, 255, 100, 255};
    SDL_Surface *text_surf = TTF_RenderText_Blended_Wrapped(timer_font, s.data(), col, -1);
    if(text_surf == nullptr)
        return;
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surf);

    Rect grect = map.GetScreenSize();
    Uint32 x, y;
    Converter::ScreenPercentToPixel((grect.x + grect.w)/2, (grect.y + grect.h)/2, &x, &y);
    SDL_Rect rect = { x-text_surf->w/2, y-text_surf->h/2, text_surf->w, text_surf->h };
    
    SDL_FreeSurface(text_surf);
    SDL_RenderCopy(renderer, text_texture, nullptr, &rect);
    SDL_DestroyTexture(text_texture);
}


void Event_Game::Action(float sPast)
{
    ClearTop();

    if(gameStartIn == 0)
    {
        bool one_crash = PlayerAction(*one, sPast);
        bool two_crash = PlayerAction(*two, sPast);

        // both players was alive, one crash, two alive
        if(!gameNeedRestart && one_crash && !two_crash)
        {
            two->ScoresUp();
            two->GetSnake()->SetSuperGhost(true);
        }
        // the same, but one alive and two crash
        if(!gameNeedRestart && !one_crash && two_crash)
        {
            one->ScoresUp();
            one->GetSnake()->SetSuperGhost(true);
        }
        if(!gameNeedRestart && (one_crash || two_crash))
        {
            gameNeedRestart = true;
            gameRestartIn = 3;
        }
    }
    else
        TimerToGame(sPast);

    if(gameNeedRestart)
    {
        gameRestartIn -= sPast;
        if(gameRestartIn <= 0)
            Restart();

        Effect *ef = one->GetSnake()->GetCrashEffect();
        if(ef != nullptr)
        {
            ef->Move(sPast);
            ef->Draw(renderer);
        }
        ef = two->GetSnake()->GetCrashEffect();
        if(ef != nullptr)
        {
            ef->Move(sPast);
            ef->Draw(renderer);
        }
    }

    DrawScores();
}
