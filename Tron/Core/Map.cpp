#include "Map.h"


CMap::CMap()
{
    _texture = nullptr;
    _matrix = nullptr;
}
CMap::CMap(const Rect &screenSize, Point worldSize)
{
    _screenSize = screenSize;
    _worldSize = worldSize;
    _worldSizeW = Uint32(_worldSize.x);
    _matrix = new Uint8[Uint32(_worldSize.x)*Uint32(_worldSize.y)];
}
CMap::~CMap()
{
    if(_texture != nullptr)
        SDL_DestroyTexture(_texture);
    if(_matrix != nullptr)
        delete [] _matrix;
}

void CMap::SetScreenSize(const Rect &size)
{
    _screenSize = size;
}
void CMap::SetWorldSize(Point size)
{
    _worldSize = size;
    _worldSizeW = Uint32(_worldSize.x);
    if(_matrix != nullptr)
        delete [] _matrix;
    _matrix = new Uint8[Uint32(_worldSize.x)*Uint32(_worldSize.y)];
}
void CMap::LoadTexture(SDL_Renderer *rnd, const string &path)
{
    if(_texture != nullptr)
    {
        SDL_DestroyTexture(_texture);
        _texture = nullptr;
    }
    SDL_Surface *surf = SDL_LoadBMP(path.data());
    if(surf == nullptr)
        throw Exception("Can't load map texture");
    _texture = SDL_CreateTextureFromSurface(rnd, surf);
    SDL_FreeSurface(surf);
    if(_texture == nullptr)
        throw Exception("Can't load map texture");
}


Point CMap::GetWorldSize() const
{
    return _worldSize;
}
Rect CMap::GetScreenSize() const
{
    return _screenSize;
}
void CMap::SetPoint(Uint32 x, Uint32 y, Uint8 val)
{
    _matrix[_worldSizeW * y + x] = val;
}
Uint8 CMap::GetPoint(Uint32 x, Uint32 y) const
{
    return _matrix[_worldSizeW * y + x];
}


void CMap::Draw(SDL_Renderer *rnd)
{
    if(_texture == nullptr)
        throw Exception("No map texture");
    
    Uint32 x, y, w, h;
    Converter::ScreenPercentToPixel(_screenSize.x, _screenSize.y, &x, &y);
    Converter::ScreenPercentToPixel(_screenSize.w, _screenSize.h, &w, &h);
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderCopy(rnd, _texture, nullptr, &rect);
}
void CMap::Clear()
{
    memset(_matrix, Map::PointType.Free, Uint32(_worldSize.x)*Uint32(_worldSize.y));
}
