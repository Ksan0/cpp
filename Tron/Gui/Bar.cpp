#include "Bar.h"

Bar::Bar()
{
    _texture = nullptr;
}
Bar::Bar(const Rect &pos)
{
    _texture = nullptr;
    SetPos(pos);
}
Bar::~Bar()
{
    if(_texture != nullptr)
        SDL_DestroyTexture(_texture);
}

void Bar::SetPos(const Rect &pos)
{
    _pos = pos;
}
void Bar::SetVal(float val)
{
    _val = val;
}
void Bar::SetMaxVal(float maxval)
{
    _maxval = maxval;
}
void Bar::LoadTexture(SDL_Renderer *rnd, const string &path)
{
    SDL_Surface *surf = SDL_LoadBMP(path.data());
    if(surf == nullptr)
        throw Exception("Can't load bar texture");
    _texture = SDL_CreateTextureFromSurface(rnd, surf);
    SDL_FreeSurface(surf);
    if(_texture == nullptr)
        throw Exception("Can't load bar texture");
}


float Bar::GetVal()
{
    return _val;
}
float Bar::GetMaxVal()
{
    return _maxval;
}


void Bar::Draw(SDL_Renderer *renderer)
{
    if(_texture == nullptr)
        throw Exception("No bar texture");
    
    Uint32 ux, uy, uw, uh;
    Converter::ScreenPercentToPixel(_pos.x, _pos.y, &ux, &uy);
    Converter::ScreenPercentToPixel(_pos.w, _pos.h, &uw, &uh);

    Sint32 x = ux, y = uy, w = uw, h = uh;
    w = Sint32(float(w) * _val / _maxval);


    auto initto = [x, y, w, h]() -> SDL_Rect
    {
        if(w >= 0)
        {
            SDL_Rect r = {x, y, w, h};
            return r;
        }

        SDL_Rect r = { x+w, y, -w, h };
        return r;
    };
    auto initfrom = [w](Uint32 t_w, Uint32 t_h, float p) -> SDL_Rect
    {
        if(w >= 0)
        {
            SDL_Rect r = {0, 0, int(float(t_w) * p), 32};
            return r;
        }

        int need_w = int(float(t_w) * p);
        SDL_Rect r = {t_w - need_w, 0, need_w, t_h};
        return r;
    };

    SDL_Rect to = initto();
    SDL_Rect from = initfrom(256, 32, _val/_maxval);
    SDL_RenderCopy(renderer, _texture, &from, &to);
}
