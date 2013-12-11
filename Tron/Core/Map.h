#ifndef _CORE_MAP_H
#define _CORE_MAP_H


#include <SDL.h>
#include <string>
#include "Object.h"
#include <string.h>
#include "../Primitives.h"
#include "Converter.h"
using std::string;


namespace Map
{
    typedef struct
    {
        static const Uint8      Free    = 0,
                                Snake   = 1,
                                Bonus   = 2;
    } _PointType;
    
    const _PointType PointType;
}


class CMap// : Object
{
    
public:
    CMap();
    CMap(const Rect &screenSize, Point worldSize);
    ~CMap();
    
    void SetScreenSize(const Rect &size);
    void SetWorldSize(Point size);
    void LoadTexture(SDL_Renderer *rnd, const string &path);
    void Draw(SDL_Renderer *rnd);
    void Clear();

    Point GetWorldSize() const;
    Rect GetScreenSize() const;

    void SetPoint(Uint32 x, Uint32 y, Uint8 val);
    Uint8 GetPoint(Uint32 x, Uint32 y) const;
private:
    Rect _screenSize;
    Point _worldSize;
    Uint8 *_matrix; // delete at destructor
    Uint32 _worldSizeW;

    SDL_Texture *_texture; // delete at destructor
};


#endif
